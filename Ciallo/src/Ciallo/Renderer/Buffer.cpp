
#include "clpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpneGL/OpenGLBuffer.h"
namespace Ciallo
{
	Ref<VertexBuffer> VertexBuffer::create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(size); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
	Ref <IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size); break;
		}

		return nullptr;
	}
}