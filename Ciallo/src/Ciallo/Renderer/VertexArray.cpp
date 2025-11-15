
#include "clpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform//OpneGL/OpenGLVertexArray.h"

namespace Ciallo
{
	Ref<VertexArray> VertexArray::Create()
	{

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>(); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
}
