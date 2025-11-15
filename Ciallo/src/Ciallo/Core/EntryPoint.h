#pragma once
#include "Ciallo/Debug/Instrumentor.h"

#if HZ_PLATFORM_WINDOWS

extern Ciallo:: Application* Ciallo::CreateApplication();


int main(int argc, char** argv)
{
	Ciallo::Log::init();

	HZ_PROFILE_BEGIN_SESSION("Startup", "CialloProfile-Startup.json");
	auto app = Ciallo::CreateApplication();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Startup", "CialloProfile-Runtime.json");
	app->run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Startup", "CialloProfile-Shutout.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif