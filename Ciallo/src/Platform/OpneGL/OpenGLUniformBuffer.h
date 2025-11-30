#pragma once

#include "Ciallo/Renderer/UniformBuffer.h"

namespace Ciallo
{
	class OpenGLUniformBuffer:public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t size,uint32_t binding);
		~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset) override;


	private:
	 	uint32_t m_RendererID;
	};
}