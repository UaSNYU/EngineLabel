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

include "Ciallo/vendor/GLAD"
include "Ciallo/vendor/GLFW"
include "Ciallo/vendor/imgui"

project "Ciallo"
   location "Ciallo"
   kind "SharedLib"
   language "C++"

   targetdir ("bin/" .. outputdir .. "/%{prj.name}")
   objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   pchheader "clpch.h"
   pchsource "Ciallo/src/clpch.cpp"
   
   files
   {
	   "%{prj.name}/src/**.h",
       "%{prj.name}/src/**.cpp"
   }

   includedirs
   {
	   "%{prj.name}/src",
	   "%{prj.name}/vendor/spdlog/include",
	   "%{IncludeDir.GLFW}",
	   "%{IncludeDir.GLAD}",
	   "%{IncludeDir.ImGui}"
   }

   links
   {
	    "ImGui",
	    "GLAD",
	    "GLFW",
		"opengl32.lib"
   }

   filter "system:windows"
      cppdialect "C++17"
	  staticruntime "off"
	  systemversion "latest"
	  buildoptions {"/utf-8"}

	  defines
	  {
		  "HZ_BUILD_DLL",
		  "HZ_PLATFORM_WINDOWS",
		  "GLFW_INCLUDE_NONE"
	  }

	  postbuildcommands
	  {
          ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	  }


	  filter "configurations:Debug"
	     defines "HZ_DEBUG"
		 buildoptions "/MDd"
		 symbols "On"

	  filter "configurations:Release"
	     defines "HZ_RELEASE"
		 buildoptions "/MD"
		 optimize "On"

	  filter "configurations:Dist"
	     defines "HZ_DIST"
		 buildoptions "/MD"
		 optimize "On"

project "Sandbox"
   location "Sandbox"
   kind "consoleApp"
   language "C++"

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
	   "Ciallo/src"
   }

   filter "system:windows"
      cppdialect "C++17"
	  staticruntime "off"
	  systemversion "latest"
	  buildoptions {"/utf-8"}

	  defines
	  {
		  "HZ_PLATFORM_WINDOWS",
		  "_WINDLL"
	  }

	  filter "configurations:Debug"
	     defines "HZ_DEBUG"
		 symbols "On"

	  filter "configurations:Release"
	     defines "HZ_RELEASE"
		 optimize "On"

	  filter "configurations:Dist"
	     defines "HZ_DIST"
		 optimize "On"