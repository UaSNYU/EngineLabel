#include "clpch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Ciallo
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b,color.a);
		
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t IndexCount)
	{
		glDrawElements(GL_TRIANGLES, IndexCount==0?vertexArray->GetIndexBuffer()->GetCount():IndexCount, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);


	}
	void OpenGLRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}
}