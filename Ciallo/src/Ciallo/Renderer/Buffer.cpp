
#include "clpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpneGL/OpenGLBuffer.h"
namespace Ciallo
{


	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size); break;
		}

		return nullptr;
	}
}