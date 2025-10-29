#include "clpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

namespace Ciallo
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_path(path)
	{
		int channels,height,width;
		stbi_uc* data = stbi_load(m_path.c_str(),&width,&height,&channels, 4);

		m_width  = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;

		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		HZ_CORE_ASSERT(internalFormat&dataFormat, "Format not supported!");
	
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID,1,GL_RGBA8,width,height);

		glTextureParameteri(m_RendererID,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		

		glTextureSubImage2D(m_RendererID,0,0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,data);
		stbi_image_free(data);

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}