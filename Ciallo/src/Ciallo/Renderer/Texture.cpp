#include "clpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpneGL/OpenGLTexture.h"

namespace Ciallo
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreatRef<OpenGLTexture2D>(path); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE: HZ_CORE_ASSERT(false, "NONE IS NOT SUPPORT!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreatRef<OpenGLTexture2D>(width,height); break;
		}

		HZ_CORE_ASSERT(false, "Unknow API!!");
		return nullptr;
	}
}