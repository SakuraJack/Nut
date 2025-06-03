project "Nut"
    kind "StaticLib"

    local targetDirStr = "../bin/" .. outputdir .. "/%{prj.name}"
    local objDirStr = "../bin-int/" .. outputdir .. "/%{prj.name}"
    targetdir (targetDirStr)
    objdir (objDirStr)

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
        defines { "NT_PLATFORM_WINDOWS" }
    
    filter "configurations:Debug"
        optimize "Off"
        symbols "On"
        AddExternalDependencies("Debug")
        defines { "NT_DEBUG" }
    
    filter "configurations:Release"
        optimize "On"
        symbols "Off"
        AddExternalDependencies("Release")
        defines { "NT_RELEASE" }

    filter "configurations:Dist"
        optimize "On"
        symbols "Off"
        AddExternalDependencies("Release")
        defines { "NT_DIST" }
