#pragma once

#if HZ_PLATFORM_WINDOWS

extern Ciallo:: Application* Ciallo::CreateApplication();


int main(int argc, char** argv)
{
	Ciallo::Log::init();
	HZ_CORE_WARN("Initializing Ciallo");
	int a = 5;

	HZ_CLIENT_INFO("hello world={0}",a);

	auto app = Ciallo::CreateApplication();
	app->run();
	delete app;
}

#endif