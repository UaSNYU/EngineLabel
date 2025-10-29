
#include "clpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Ciallo
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		:m_windowHandle(window)
	{

		HZ_CORE_ASSERT(m_windowHandle, "m_window is not null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		HZ_CORE_INFO("OpenGL Render:{0}---{1} ", (char*)glGetString(GL_VENDOR),(char*)glGetString(GL_RENDERER));

	
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

}