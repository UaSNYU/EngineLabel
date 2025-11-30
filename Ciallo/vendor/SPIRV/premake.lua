project "SPRIV"
    kind "staticlib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    targetdir ("bin-int/" .. outputdir .. "/%{prj.name}")

   

    files
    {
        "src/**.cpp",
        "src/**.h",

        "include/**.h"
    }

    includedirs
    {
        "include"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"
        defines {"YAML_CPP_STATIC_DEFINE"} 

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

