#pragma once

#include "Ciallo/Renderer/Texture.h"
#include "glad/glad.h"

namespace Ciallo
{
	class OpenGLTexture2D: public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_width; };
		virtual uint32_t GetHeight() const override { return m_height; };

		virtual void Bind(uint32_t slot) const override;

	private:
        uint32_t m_width, m_height;
		uint32_t m_RendererID;
		std::string m_path;
	};

}