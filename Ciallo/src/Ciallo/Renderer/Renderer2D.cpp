#include "clpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Ciallo
{

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTexureSlots = 32;

		Ref<Shader> m_TextureShader;
		Ref<VertexArray> m_QuadVA;
		Ref<VertexBuffer> m_QuadVB;
		Ref<Texture2D> m_WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];
	

		Renderer2D::Statistics stats;
	};

	static Renderer2DStorage s_Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		s_Data.m_QuadVA = VertexArray::Create();

		uint32_t* indices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (int i = 0; i < s_Data.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset+=4;
		}

		s_Data.m_QuadVB = VertexBuffer::create(s_Data.MaxVertices*sizeof(QuadVertex));
		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::create(indices,s_Data.MaxIndices);

		BufferLayout layout = {
			{"a_position",ShaderDateType::Float3},
			{"a_color",ShaderDateType::Float4},
			{"a_textcoord",ShaderDateType::Float2},
			{"a_textindex",ShaderDateType::Float},
			{"a_tilingFactor",ShaderDateType::Float}
		};

		s_Data.m_QuadVB->SetLayout(layout);

		s_Data.m_QuadVA->AddVertexBuffer(s_Data.m_QuadVB);
		s_Data.m_QuadVA->AddIndexBuffer(indexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		delete[] indices;

		s_Data.m_WhiteTexture=Texture2D::Create(1, 1);
		uint32_t data = 0xffffffff;
		s_Data.m_WhiteTexture->SetData(&data,sizeof(uint32_t));

		int samplers[s_Data.MaxTexureSlots];
		for (int i = 0; i < s_Data.MaxTexureSlots; i++)
		{
            samplers[i] = i;
		}

		s_Data.m_TextureShader = Shader::Create("res/Shaders/TextureShader.shader");
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetIntArray("u_texture",samplers, s_Data.MaxTexureSlots);

		s_Data.QuadVertexPositions[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,0.5f,0.0f ,1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,0.5f,0.0f,1.0f };
	}

	void Renderer2D::Shutdown()
	{
	}
	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_viewProjection", viewProjection);

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;

		//Clear TexSlots
		memset(s_Data.TextureSlots.data(), 0, s_Data.TextureSlots.size() * sizeof(uint32_t));
		s_Data.TextureSlots[0] = s_Data.m_WhiteTexture;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data.m_TextureShader->Bind();
		s_Data.m_TextureShader->SetMat4("u_viewProjection", camera.GetViewProjectionMatrix());

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;

		//Clear TexSlots
		memset(s_Data.TextureSlots.data(), 0, s_Data.TextureSlots.size() * sizeof(uint32_t));
		s_Data.TextureSlots[0] = s_Data.m_WhiteTexture;
	}
	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.m_QuadVB->Setdata(s_Data.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.m_QuadVA,s_Data.QuadIndexCount);

		s_Data.stats.DrawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;

	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		const float textureindex = 0.0f;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		if(s_Data.QuadIndexCount>=Renderer2DStorage::MaxIndices)
			FlushAndReset();
	
		const float textureindex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));


		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor, glm::vec4 TilingColor)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture, TilingFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor, glm::vec4 TilingColor)
	{
		glm::vec2 TextureCoords[] =
		{
			{0.0f,0.0f},
			{1.0f,0.0f },
			{1.0f,1.0f},
			{0.0f,1.0f},
		};

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureindex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*texture.get() == *s_Data.TextureSlots[i].get())
			{
				textureindex = (float)i;
				break;
			}
		}

		if (textureindex == 0.0f)
		{
			textureindex=(float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex]= texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));


		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
		/*s_Data.m_TextureShader->SetFloat4("u_color", TilingColor);
		s_Data.m_TextureShader->SetFloat("u_TilingFactor", TilingFactor);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));

		texture->Bind();

		s_Data.m_TextureShader->SetMat4("u_transform", transform);
		s_Data.m_QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVA);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor , glm::vec4 TilingColor )
	{
		DrawQuad({ position.x,position.y,0.0f }, size, subtexture, TilingFactor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor , glm::vec4 TilingColor )
	{
		glm::vec2* TextureCoords = subtexture->GetTexCoords();
		Ref<Texture2D> texture = subtexture->GetTexture();

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureindex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*texture.get() == *s_Data.TextureSlots[i].get())
			{
				textureindex = (float)i;
				break;
			}
		}

		if (textureindex == 0.0f)
		{
			textureindex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));


		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}


////////////////////////////////////ROTATED QUADS/////////////////////////////////////////
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad({position.x,position.y,0.0f}, rotation, size, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();


		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		const float textureindex = 0.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor, glm::vec4 TilingColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, rotation, size, texture);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float TilingFactor, glm::vec4 TilingColor)
	{

		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureindex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*texture.get() == *s_Data.TextureSlots[i].get())
			{
				textureindex = (float)i;
				break;
			}
		}

		if (textureindex == 0.0f)
		{
			textureindex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			*glm::rotate(glm::mat4(1.0f),glm::radians(rotation),glm::vec3(0.0f,0.0f,1.0f))
			*glm::scale(glm::mat4(1.0f),glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform*s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;

		/*s_Data.m_TextureShader->SetFloat4("u_color", TilingColor);
		s_Data.m_TextureShader->SetFloat("u_TilingFactor", TilingFactor);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));
		transform = glm::rotate(transform, rotation, { 0.0f,0.0f,1.0f });

		texture->Bind();

		s_Data.m_TextureShader->SetMat4("u_transform", transform);
		s_Data.m_QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVA);*/
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor , glm::vec4 TilingColor )
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, rotation, size, subtexture,TilingFactor, TilingColor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float TilingFactor, glm::vec4 TilingColor )
	{
		glm::vec2* TextureCoords = subtexture->GetTexCoords();
		Ref<Texture2D> texture = subtexture->GetTexture();


		if (s_Data.QuadIndexCount >= Renderer2DStorage::MaxIndices)
			FlushAndReset();

		const glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureindex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*texture.get() == *s_Data.TextureSlots[i].get())
			{
				textureindex = (float)i;
				break;
			}
		}

		if (textureindex == 0.0f)
		{
			textureindex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = TextureCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureindex;
		s_Data.QuadVertexBufferPtr->TilingFactor = TilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;

		s_Data.stats.QuadCount++;

	}


	void Renderer2D::ResetStats()
	{
		memset(&s_Data.stats, 0, sizeof(Statistics));
	}


	Renderer2D::Statistics& Renderer2D::GetStatistics()
	{
		return s_Data.stats;
	}

}