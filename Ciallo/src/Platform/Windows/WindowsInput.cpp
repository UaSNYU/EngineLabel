#include "clpch.h"
#include "WindowsInput.h"

#include "GLFW/glfw3.h"
#include "Ciallo/Application.h"

namespace Ciallo
{
	Input* Input::s_Instance = new WindowsInput();

	bool Ciallo::WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window =static_cast<GLFWwindow*>( Application::Get().GetWindow().GetNativeWindow());

		auto state= glfwGetKey(window, keycode);

		return state == GLFW_PRESS||state==GLFW_REPEAT;
	}
	bool WindowsInput::IsMouseButtonImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}
	float WindowsInput::GetMouseXImpl()
	{
		auto[x,y]= GetMousePosImpl();

		return x;
	}
	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();

		return y;
	}
	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
}