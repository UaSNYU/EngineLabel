#include "clpch.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"

namespace Ciallo
{

	namespace Utils {
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum HazelFBTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::RGBA8:       return GL_RGBA8;
			case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			HZ_CORE_ASSERT(false);
			return 0;
		}

	}

	//DELETE
	void FrameBuffer::ColorFrameBufferCopy(Ref<FrameBuffer>& source, Ref<FrameBuffer>& dest)
	{
		// 绑定读(MSAA) 和 写(Intermediate/View)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, source->GetRendererID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->GetRendererID());

		auto width = source->GetSpecification().Width;
		auto height = source->GetSpecification().Height;

		// ---------------------------------------------
		// 第一步：解析颜色附件 (Attachment 0)
		// ---------------------------------------------
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, width, height,
			0, 0, width, height,
			GL_COLOR_BUFFER_BIT, GL_LINEAR); // 颜色可以用 Linear

		// ---------------------------------------------
		// 第二步：解析实体 ID 附件 (Attachment 1 - RED_INTEGER)
		// ---------------------------------------------
		// 只有当你的 FrameBuffer 确实有两个以上附件时才执行这一步
		// 在 Hazel 架构中，通常 Attachment 1 是 RED_INTEGER
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, width, height,
			0, 0, width, height,
			GL_COLOR_BUFFER_BIT, GL_NEAREST); // 整数必须用 Nearest！

		// 解绑
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	static uint32_t s_MaxFrameBufferSize = 8124;

	OpenGLFrameBuffer::OpenGLFrameBuffer(FrameBufferSpecification& spec)
		:m_Spec(spec)
	{

		for (auto format : m_Spec.m_Attachements.Attachments)
		{
			if (!Utils::IsDepthFormat(format.m_TextureFormat))
			{
				m_ColorAttachmentSpecification.emplace_back(format);
			}
			else
			{
				m_DepthAttachmentSpecification =format;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColroAttachments.size(), m_ColroAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() 
	{
		return m_Spec;
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColroAttachments.size(),m_ColroAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

            m_ColroAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		//Attachments--Settings
		bool multisampled = m_Spec.Samples > 1;
		if (m_ColorAttachmentSpecification.size())
		{
			m_ColroAttachments.resize(m_ColorAttachmentSpecification.size());
			Utils::CreateTextures(multisampled, m_ColroAttachments.data(), m_ColroAttachments.size());

			for (size_t i=0; i < m_ColroAttachments.size(); i++)
			{
				switch (m_ColorAttachmentSpecification[i].m_TextureFormat)
				{
					case FrameBufferTextureFormat::RGBA8:
					{
						Utils::BindTexture(multisampled, m_ColroAttachments[i]);
						Utils::AttachColorTexture(m_ColroAttachments[i], m_Spec.Samples, GL_RGBA8,GL_RGBA, m_Spec.Width, m_Spec.Height, i);
						break;
					}
					case FrameBufferTextureFormat::RED_INTEGER:
					{
						Utils::BindTexture(multisampled, m_ColroAttachments[i]);
						Utils::AttachColorTexture(m_ColroAttachments[i], m_Spec.Samples, GL_R32I, GL_RED_INTEGER, m_Spec.Width, m_Spec.Height, i);
						break;
					}
				}
			}
		}

		if (m_DepthAttachmentSpecification.m_TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multisampled, &m_DepthAttachment, 1);
			Utils::BindTexture(multisampled, m_DepthAttachment);

			switch (m_DepthAttachmentSpecification.m_TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
			{
				Utils::AttachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
				break;
			}
			}
		}

		if (m_ColroAttachments.size() > 1)
		{
			HZ_CORE_ASSERT(m_ColorAttachmentSpecification.size() <= 4, "Multiple color(<=4) attachments are not supported!");
			GLenum buffers[4] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColroAttachments.size(), buffers);
		}
		else if (m_ColroAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}


		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width <= 0 || height <= 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			HZ_CORE_WARN("Attmpt to Resize :{0}/{1}", width,height);
			return;
		}

		m_Spec.Width = width;
		m_Spec.Height = height;
		Invalidate();
		
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachment, int x, int y)
	{
		HZ_CORE_ASSERT(attachment<m_ColroAttachments.size());

		int PixelData;
		glReadBuffer(GL_COLOR_ATTACHMENT0+attachment);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &PixelData);
		return PixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachment,int value)
	{
		HZ_CORE_ASSERT(attachment < m_ColroAttachments.size());

		auto& spec = m_ColorAttachmentSpecification[attachment];
	
		glClearTexImage(m_ColroAttachments[attachment],0,Utils::HazelFBTextureFormatToGL(spec.m_TextureFormat),GL_INT,&value);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}