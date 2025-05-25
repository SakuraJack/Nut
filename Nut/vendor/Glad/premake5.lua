project "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
	targetdir ("bin/%{cfg.buildcfg}/")
    objdir ("bin-int/%{cfg.buildcfg}/")

	files
	{
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

	filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"