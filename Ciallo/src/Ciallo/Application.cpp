#include "clpch.h"
#include "Application.h"
#include "Input.h"
#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"
#include "GLFW//glfw3.h"
#include "Core/TimeStep.h"

namespace Ciallo {
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer=new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		Renderer::Init();
	}

	Application::~Application()
	{

	}

	void Application::pushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::pushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	    overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		HZ_CORE_TRACE("{0}",e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_handled)
			{
				break;
			}
		}
	}

	void Application::run()
	{
		while (m_Running)
		{
			float time=(float)glfwGetTime();
            TimeStep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
			
			if (!minimize)
			{
				for (auto layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			for (auto layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}


	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetHeight() == 0 || e.GetWidth() == 0)
		{
			minimize = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		minimize = false;

		return false;
	}

}

