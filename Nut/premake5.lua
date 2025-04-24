project "Nut"
    kind "ConsoleApp"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ntpch.h"
    pchsource "src/ntpch.cpp"

    files {
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs { "src/", "vendor/", }
    
    defines { "GLM_FORCE_DEPTH_ZERO_TO_ONE" }
    
    filter "system:windows"
    systemversion "latest"
    
    filter "configurations:Debug"
    optimize "Off"
    symbols "On"
    IncludeDependencies("Debug")
    postbuildcommands {
        '{COPY} "../Nut/vendor/Assimp/bin/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
    }
    
    filter "configurations:Release"
    optimize "On"
    symbols "Off"
    IncludeDependencies("Release")
    postbuildcommands {
        '{COPY} "../Nut/vendor/Assimp/bin/Release/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
    }
