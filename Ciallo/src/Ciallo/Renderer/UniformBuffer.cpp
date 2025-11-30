#include "clpch.h"
#include "UniformBuffer.h"
#include "Renderer.h"
#include "Platform/OpneGL/OpenGLUniformBuffer.h"

namespace Ciallo
{

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLUniformBuffer>(size,binding); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
}