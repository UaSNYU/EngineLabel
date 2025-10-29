#pragma once

#include "Ciallo/Renderer/VertexArray.h"

namespace Ciallo
{
	class OpenGLVertexArray:public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer> vertexBuffer) override;
		virtual void AddIndexBuffer(const  Ref<IndexBuffer> vertexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffer() const {return m_VertexBuffers;}
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const {return m_IndexBuffer;}

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}