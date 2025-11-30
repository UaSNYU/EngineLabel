#pragma once

#include "Ciallo/Renderer/FrameBuffer.h"

namespace Ciallo
{
	class OpenGLFrameBuffer :public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual FrameBufferSpecification& GetSpecification() override;

		void Invalidate();

		virtual void Bind()override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual int ReadPixel(uint32_t attachment, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachment,int value) override;

		virtual uint32_t GetColorAttachment(uint32_t index = 0) override { HZ_CORE_ASSERT(index<m_ColroAttachments.size()); return m_ColroAttachments[index]; }

		virtual uint32_t GetRendererID() { return m_RendererID; };

		virtual void Unbind()override;

	private:
		bool HasResized = false;

		uint32_t m_RendererID=0;
		FrameBufferSpecification m_Spec;

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecification;
		FrameBufferTextureSpecification m_DepthAttachmentSpecification ;

		std::vector<uint32_t> m_ColroAttachments;
		uint32_t m_DepthAttachment;
	};

}