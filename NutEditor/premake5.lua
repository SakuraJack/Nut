project "NutEditor"
    kind "ConsoleApp"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    links { "Nut" }

    defines { "GLM_FORCE_DEPTH_ZERO_TO_ONE" }

    files {
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",

        -- Shaders
        "Resources/Shaders/**.glsl",
    }

    includedirs {
        "src/",
        "../Nut/src/",
        "../Nut/vendor/"
    }

    filter "system:windows"
        systemversion "latest"

        defines { "NT_PLATFORM_WINDOWS" }

    filter { "system:windows", "configurations:Debug" }
        postbuildcommands {
            '{COPY} "../Nut/vendor/assimp/bin/Debug/assimp-vc143-mtd.dll" "%{cfg.targetdir}"',
        }

    filter { "system:windows", "configurations:Release or configurations:Dist" }
        postbuildcommands {
            '{COPY} "../Nut/vendor/assimp/bin/Release/assimp-vc143-mt.dll" "%{cfg.targetdir}"',
        }

    filter "configurations:Debug"
        symbols "On"
        defines { "NT_DEBUG" }
        AddExternalDependencies("Debug")

    filter "configurations:Release"
        optimize "On"
        symbols "Default"
        defines { "NT_RELEASE" }
        AddExternalDependencies("Release")

    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"
        defines { "NT_DIST" }
        AddExternalDependencies("Release")