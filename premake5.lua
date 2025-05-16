include "Dependencies.lua"

workspace "Nut"
    configurations { "Debug", "Release" }
    targetdir "build"
    startproject "Nut"

    language "C++"
    cppdialect "C++20"

    configurations { "Debug", "Release" }

    flags { "MultiProcessorCompile" }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
    }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    filter "action:vs*"
        linkoptions { "/ignore:4099" } -- 不显示no PDB的warning
        disablewarnings { "4068" }

    filter "language:C++ or language:C"
		architecture "x86_64"

    filter "configurations:Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
        symbols "Default"

    architecture "x86_64"

    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

group "Dependencies"
    include "Nut/vendor/GLFW"
    include "Nut/vendor/Glad"
    include "Nut/vendor/imgui"
group ""

group "Core"
    include "Nut"
group ""