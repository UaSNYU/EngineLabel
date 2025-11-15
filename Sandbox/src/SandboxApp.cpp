#include <Ciallo.h>
#include "testexample/Sandbox2D.h"
#include "Ciallo/Core/EntryPoint.h"

class Sandbox : public Ciallo::Application
{
public:
	Sandbox()
		:Application("Sandbox")
	{
		//pushLayer(new Sandbox2D());
		//pushLayer(new GameLayer());
	}

	~Sandbox() {}

};

Ciallo::Application* Ciallo::CreateApplication()
{
	return new Sandbox();
}