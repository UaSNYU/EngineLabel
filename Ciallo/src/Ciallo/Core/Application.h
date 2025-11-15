#pragma once

#include "Core.h"
#include "Ciallo/Events/Event.h"
#include "Ciallo/Renderer/Shader.h"
#include "Window.h"
#include "Ciallo/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Ciallo/ImGui/ImGuiLayer.h"

#include "Ciallo/Renderer/Buffer.h"
#include "Ciallo/Renderer/VertexArray.h"
#include "Ciallo/Renderer/OrthographicCamera.h"

namespace Ciallo{

class CIALLO_API Application
{
public:
	Application(std::string name="Ciallo Engine");
	virtual ~Application();

	void OnEvent(Event& e);

	void run();

	ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	void Close();

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

