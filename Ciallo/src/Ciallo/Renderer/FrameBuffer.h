#pragma once

#include "Ciallo/Core/Core.h"

namespace Ciallo
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,
		//Depth/Stencil
		DEPTH24STENCIL8,

		//Default
		Depth= DEPTH24STENCIL8,
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format)
			:m_TextureFormat(format){}

		FrameBufferTextureFormat m_TextureFormat=FrameBufferTextureFormat::None;
		//TODO---WRAP/Filter
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
			:Attachments(attachments){}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width ,Height;
		FrameBufferAttachmentSpecification m_Attachements;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};


	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void Bind()=0;

		virtual int ReadPixel(uint32_t attachment, int x, int y)=0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual void ClearAttachment(uint32_t attachment, int value) = 0;

		virtual uint32_t GetColorAttachment(uint32_t index=0) = 0;

		virtual uint32_t GetRendererID() = 0;

		virtual void Unbind()=0;

		virtual FrameBufferSpecification& GetSpecification() = 0;

		static Ref<FrameBuffer> Create(FrameBufferSpecification& spec);
		static void ColorFrameBufferCopy(Ref<FrameBuffer>& source, Ref<FrameBuffer>& dest);

	};
}