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
			return GL_UNSIGNED_INT;
		case ShaderDateType::Int2:
			return  GL_UNSIGNED_INT;
		case ShaderDateType::Int3:
			return  GL_UNSIGNED_INT;
		case ShaderDateType::Int4:
			return  GL_UNSIGNED_INT;
		case ShaderDateType::Bool:
			return GL_BOOL;
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> vertexBuffer)
	{
        glBindVertexArray(m_RendererID);
		vertexBuffer->bind();
		const BufferLayout &layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,element.GetComponentCount(), ShaderDataTypeToOpenGL(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)element.offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::AddIndexBuffer(const Ref<IndexBuffer> indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->bind();
		m_IndexBuffer = indexBuffer;
	}
}