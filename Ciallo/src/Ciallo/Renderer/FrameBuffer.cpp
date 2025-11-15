
#include "clpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpneGL/OpenGLFrameBuffer.h"

namespace Ciallo
{



	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferSpecification& spec)
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLFrameBuffer>(spec); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}

}