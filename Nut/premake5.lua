project "Nut"
    kind "ConsoleApp"

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
    
    filter "configurations:Debug"
    optimize "Off"
    symbols "On"
    AddExternalDependencies("Debug", targetDirStr)
    
    filter "configurations:Release"
    optimize "On"
    symbols "Off"
    AddExternalDependencies("Release", targetDirStr)
