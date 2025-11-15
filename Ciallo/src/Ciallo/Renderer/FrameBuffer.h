#pragma once

#include "Ciallo/Core/Core.h"

namespace Ciallo
{
	struct FrameBufferSpecification
	{
		uint32_t Width ,Height;

		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};


	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void Bind()=0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachment() = 0;

		virtual void Unbind()=0;

		virtual FrameBufferSpecification& GetSpecification() = 0;
		static Ref<FrameBuffer> Create(FrameBufferSpecification& spec);

	};
}