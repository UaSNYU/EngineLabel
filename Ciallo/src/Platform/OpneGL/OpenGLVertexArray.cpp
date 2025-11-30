#include "clpch.h"
#include "OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Ciallo
{

	static GLenum ShaderDataTypeToOpenGL(ShaderDateType type)
	{
		switch (type)
		{
		case ShaderDateType::Float:
			return GL_FLOAT;
		case ShaderDateType::Float2:
			return  GL_FLOAT;
		case ShaderDateType::Float3:
			return  GL_FLOAT;
		case ShaderDateType::Float4:
			return  GL_FLOAT;
		case ShaderDateType::Mat3:
			return  GL_FLOAT;
		case ShaderDateType::Mat4:
			return GL_FLOAT;
		case ShaderDateType::Int:
			return GL_INT;
		case ShaderDateType::Int2:
			return  GL_INT;
		case ShaderDateType::Int3:
			return  GL_INT;
		case ShaderDateType::Int4:
			return  GL_INT;
		case ShaderDateType::Bool:
			return GL_BOOL;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		HZ_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
		vertexBuffer->bind();
		const BufferLayout &layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			if (element.type == ShaderDateType::Int)
			{
				glVertexAttribIPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type), layout.GetStride(), (void*)element.offset);
			}
			else
			{
				glVertexAttribPointer(index,element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);		
			}

			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer> indexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->bind();
		m_IndexBuffer = indexBuffer;
	}
}