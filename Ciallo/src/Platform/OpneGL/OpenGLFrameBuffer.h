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

		virtual uint32_t GetColorAttachment() override { return m_ColorAttachment; }

		virtual void Unbind()override;

	private:
		bool HasResized = false;

		uint32_t m_RendererID=0;
		uint32_t m_ColorAttachment=0,m_DepthAttachment=0; 
		FrameBufferSpecification m_Spec;
	};

}