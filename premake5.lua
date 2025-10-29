workspace "Ciallo"
   architecture "x64"

   configurations
   {
	   "Debug",
	   "Release",
	   "Dist"
   }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)

IncludeDir = {}
IncludeDir["GLFW"] = "Ciallo/vendor/GLFW/include"
IncludeDir["GLAD"] = "Ciallo/vendor/GLAD/include"
IncludeDir["ImGui"] = "Ciallo/vendor/imgui"
IncludeDir["glm"] = "Ciallo/vendor/glm"
IncludeDir["Stb_image"] = "Ciallo/vendor/STBIMAGE"

include "Ciallo/vendor/GLAD"
include "Ciallo/vendor/GLFW"
include "Ciallo/vendor/imgui"


project "Ciallo"
   location "Ciallo"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   pchheader "clpch.h"
   pchsource "Ciallo/src/clpch.cpp"
   
   files
   {
	   "%{prj.name}/src/**.h",
       "%{prj.name}/src/**.cpp",
	   "%{prj.name}/vendor/glm/glm/**.hpp",
	   "%{prj.name}/vendor/glm/glm/**.inl",
	   "%{prj.name}/vendor/STBIMAGE/stb_image.h",
	   "%{prj.name}/vendor/STBIMAGE/stb_image.cpp"
   }

   includedirs
   {
	   "%{prj.name}/src",
	   "%{prj.name}/vendor/spdlog/include",
	   "%{IncludeDir.GLFW}",
	   "%{IncludeDir.GLAD}",
	   "%{IncludeDir.ImGui}",
	   "%{IncludeDir.glm}",
	   "%{IncludeDir.Stb_image}"
   }

   links
   {
	    "ImGui",
	    "GLAD",
	    "GLFW",
		"opengl32.lib"
   }

   filter "system:windows"
	  systemversion "latest"
	  buildoptions {"/utf-8"}

	  defines
	  {
		  "HZ_BUILD_DLL",
		  "HZ_PLATFORM_WINDOWS",
		  "GLFW_INCLUDE_NONE"
	  }

	  filter "configurations:Debug"
	     defines "HZ_DEBUG"
		 runtime "Debug"
		 symbols "On"

	  filter "configurations:Release"
	     defines "HZ_RELEASE"
		 runtime "Release"
		 optimize "On"

	  filter "configurations:Dist"
	     defines "HZ_DIST"
		 runtime "Release"
		 optimize "On"

project "Sandbox"
   location "Sandbox"
   kind "consoleApp"
   language "C++"
   cppdialect "C++17"
   staticruntime "on"

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   links
   {
	   "Ciallo"
   }

   files
   {
	   "%{prj.name}/src/**.h",
       "%{prj.name}/src/**.cpp"
   }

   includedirs
   {
	   "Ciallo/vendor/spdlog/include",
	   "Ciallo/src",
	   "%{IncludeDir.glm}",
	   "%{IncludeDir.ImGui}"
   }

   filter "system:windows"
	  systemversion "latest"
	  buildoptions {"/utf-8"}

	  defines
	  {
		  "HZ_PLATFORM_WINDOWS",
		  "_WINDLL"
	  }

	  filter "configurations:Debug"
	     defines "HZ_DEBUG"
		 runtime "Debug"
		 symbols "On"

	  filter "configurations:Release"
	     defines "HZ_RELEASE"
		 runtime "Release"
		 optimize "On"

	  filter "configurations:Dist"
	     defines "HZ_DIST"
		 runtime "Release"
		 optimize "On"