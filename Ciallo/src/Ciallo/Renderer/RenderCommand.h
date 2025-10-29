#pragma once

#include "RendererAPI.h"

namespace Ciallo
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			m_RenderAPI->Init();
		}

		inline static void SetViewPort(uint32_t x, uint32_t y,uint32_t width, uint32_t height)
		{
			m_RenderAPI->SetViewPort(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
            m_RenderAPI->SetClearColor(color);
		}
		inline static void Clear() {
            m_RenderAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
            m_RenderAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* m_RenderAPI;
	};
}