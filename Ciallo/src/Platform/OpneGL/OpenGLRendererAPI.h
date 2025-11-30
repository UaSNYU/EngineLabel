#pragma once


#include "Ciallo/Renderer/RendererAPI.h"


namespace Ciallo
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void Init() override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t IndexCount = 0) override;
		virtual void DrawLines(const const Ref<VertexArray>& vertexArray, uint32_t vertexcount = 0)override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetLineWidth(float width)override;

	};

}