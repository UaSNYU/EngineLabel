#pragma once

#include "Ciallo/Renderer/Texture.h"
#include "glad/glad.h"

namespace Ciallo
{
	class OpenGLTexture2D: public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const uint32_t width, const uint32_t height);
		~OpenGLTexture2D();

		virtual void SetData(void* data, uint32_t size) const override;

		virtual uint32_t GetWidth() const override { return m_width; };
		virtual uint32_t GetHeight() const override { return m_height; };

		virtual bool operator==(Texture& other) const override
		{
			return m_RendererID== static_cast<OpenGLTexture2D&>(other).m_RendererID;
		}

		virtual void Bind(uint32_t slot=0) const override;

	private: 
        uint32_t m_width, m_height;
		uint32_t m_RendererID;
		std::string m_path;

		GLenum m_internalFormat, m_dataFormat;
	};

}