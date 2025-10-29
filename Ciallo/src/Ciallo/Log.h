#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Ciallo {
	class CIALLO_API Log
	{
	public:
		static void init();
		inline static Ref<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& getClientLogger() { return s_ClientLogger; }
	private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
	};

}

//core log macros
#define HZ_CORE_FATAL(...) ::Ciallo::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define HZ_CORE_ERROR(...) ::Ciallo::Log::getCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_WARN(...) ::Ciallo::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_INFO(...) ::Ciallo::Log::getCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_TRACE(...) ::Ciallo::Log::getCoreLogger()->trace(__VA_ARGS__)

//core log macros
#define HZ_CLIENT_FATAL(...) ::Ciallo::Log::getCoreLogger()->fatal(__VA_ARGS__)
#define HZ_CLIENT_ERROR(...) ::Ciallo::Log::getCoreLogger()->error(__VA_ARGS__)
#define HZ_CLIENT_WARN(...) ::Ciallo::Log::getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CLIENT_INFO(...) ::Ciallo::Log::getCoreLogger()->info(__VA_ARGS__)
#define HZ_CLIENT_TRACE(...) ::Ciallo::Log::getCoreLogger()->trace(__VA_ARGS__)

