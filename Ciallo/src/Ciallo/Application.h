#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"


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
	bool m_Running = true;
	static Application* s_Instance;
	
	
	bool OnWindowClose(WindowCloseEvent& e);
};
    
    Application* CreateApplication();
}

