function firstToUpper(str)
    return (str:gsub("^%l", string.upper))
end
Vulkan_SDK = os.getenv("Vulkan_SDK")

Dependencies = {
    Vulkan = {
        LibName = "vulkan-1",
        IncludeDir = "%{Vulkan_SDK}/Include/",
        LibDir = "%{Vulkan_SDK}/Lib/",
    },

    ShaderC = {
        LibName = "shaderc_combined",
        DebugLibName = "shaderc_combinedd",
        Configurations = "Debug,Release",
    },

    SPIRVCrossCore = {
        LibName = "spirv-cross-core",
        DebugLibName = "spirv-cross-cored",
        Configurations = "Debug,Release",
    },

    SPIRVCrossGLSL = {
        LibName = "spirv-cross-glsl",
        DebugLibName = "spirv-cross-glsld",
        Configurations = "Debug,Release",
    },

    SPIRVTools = {
        LibName = "SPIRV-Tools",
        DebugLibName = "SPIRV-Toolsd",
        Configurations = "Debug,Release",
    },
    
    Assimp = {
        IncludeDir = "%{wks.location}/Nut/vendor/Assimp/include",
        LibName = "assimp-vc143-mt",
        DebugLibName = "assimp-vc143-mtd",
        LibDir = "%{wks.location}/Nut/vendor/Assimp/bin/%{cfg.buildcfg}/",
        DllName = "assimp-vc143-mt.dll",
        DebugDllName = "assimp-vc143-mtd.dll",
        DllDir = "%{wks.location}/Nut/vendor/Assimp/bin/%{cfg.buildcfg}/",
        Configurations = "Debug,Release"
    },

    GLFW = {
        LibName = "GLFW",
        IncludeDir = "%{wks.location}/Nut/vendor/GLFW/include",
        LibDir = "%{wks.location}/Nut/vendor/GLFW/bin/%{cfg.buildcfg}/",
    },

    GLM = {
        IncludeDir = "%{wks.location}/Nut/vendor/glm",
    },

    Entt = {
        IncludeDir = "%{wks.location}/Nut/vendor/entt/src",
    },

    Glad = {
        LibName = "Glad",
        IncludeDir = "%{wks.location}/Nut/vendor/Glad/include",
        LibDir = "%{wks.location}/Nut/vendor/Glad/bin/%{cfg.buildcfg}/",
    },

    ImGui = {
        LibName = 'ImGui',
        IncludeDir = "%{wks.location}/Nut/vendor/imgui",
        LibDir = "%{wks.location}/Nut/vendor/imgui/bin/%{cfg.buildcfg}/",
    },

    stb_image = {
        IncludeDir = "%{wks.location}/Nut/vendor/stb_image",
    },
}

function LibDependency(table, isDebug)
    local libraryName = nil
    if table.LibName ~= nil then
        libraryName = table.LibName
    end
    
    if table.DebugLibName ~= nil and isDebug then
        libraryName = table.DebugLibName
    end

    if table.LibDir ~= nil then
        libdirs { table.LibDir }
    end

    if libraryName ~= nil then
        links { libraryName }
    end
end

function DllDependency(table, isDebug, copyTargetPath)
    local dllName = nil
    if isDebug and table.DebugDllName ~= nil then
        dllName = table.DebugDllName
    elseif (not isDebug) and table.DllName ~= nil then
        dllName = table.DllName
    end

    if table.DllDir ~= nil then
        postbuildcommands {
            "{COPYFILE} " .. table.DllDir .. dllName .. " " .. copyTargetPath .. dllName,
        }
    end
end

function IncludeDependency(table)
    if table.IncludeDir ~= nil then
        externalincludedirs { table.IncludeDir }
    end
end

function AddExternalDependencies(config_name, targetPath)
    local isDebug = config_name == "Debug"
    for key, value in pairs(Dependencies) do
        if value.Configurations == nil or value.Configurations:find(config_name) then
            LibDependency(value, isDebug)
            IncludeDependency(value)
            DllDependency(value, isDebug, targetPath)
        end
    end
end