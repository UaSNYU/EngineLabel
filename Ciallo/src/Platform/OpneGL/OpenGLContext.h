#pragma once

#include "Ciallo/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Ciallo
{
	class OpenGLContext :public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:

		GLFWwindow* m_windowHandle;
	};
}