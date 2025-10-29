#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Renderer/Shader.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthographicCamera.h"

namespace Ciallo{

class CIALLO_API Application
{
public:
	Application();
	virtual ~Application();

	void OnEvent(Event& e);

	void run();

	

	void pushLayer(Layer* layer);
	void pushOverlay(Layer* layer);

	inline Window& GetWindow() {return *m_Window;}
	inline static Application& Get() { return *s_Instance; }

private:
	LayerStack m_LayerStack;
	std::unique_ptr<Window> m_Window;
	ImGuiLayer* m_ImGuiLayer;
	bool m_Running = true;
	bool minimize = false;
	static Application* s_Instance;
	float m_LastFrameTime=0.0f;

private:
	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);
};
    
    Application* CreateApplication();
}

