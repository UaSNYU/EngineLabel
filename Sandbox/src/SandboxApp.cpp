#include <Ciallo.h>
 
class ExampleLayer : public Ciallo::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate()override
	{
		//HZ_CORE_INFO("ExampleLayer::Update");
	}

	void OnEvent(Ciallo::Event& e) override
	{
		HZ_CORE_TRACE("{}", e.ToString());
	}
};

class Sandbox : public Ciallo::Application
{
public:
	Sandbox()
	{
		pushLayer(new ExampleLayer());
		pushLayer(new Ciallo::ImGuiLayer());
	}

	~Sandbox() {}

};

Ciallo::Application* Ciallo::CreateApplication()
{
	return new Sandbox();
}