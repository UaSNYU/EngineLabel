#pragma once

#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
   #ifdef HZ_BUILD_DLL
      #define CIALLO_API __declspec(dllexport)
   #else
      #define CIALLO_API __declspec(dllimport)
   #endif
#else
   #define CIALLO_API
#endif
#else
  #error Ciallo only run on Windows!
#endif

#ifdef HZ_ENABLE_ASSERTS
    #define HZ_ASSERT(x,...) {if(!(x)) {HZ_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak();}}
    #define HZ_CORE_ASSERT(x,...) {if(!(x)) {HZ_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak();}}
#else
    #define HZ_ASSERT(x,...)
    #define HZ_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1<<x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Ciallo
{

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
	using Scope = std::unique_ptr<T>;

    template<typename T,typename ... Args>
    constexpr Ref<T> CreatRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ...Args>
    constexpr Scope<T> CreatScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

}