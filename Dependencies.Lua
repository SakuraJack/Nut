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
        Configurations = "Debug,Release"
    },

    GLFW = {
        LibName = "GLFW",
        IncludeDir = "%{wks.location}/Nut/vendor/GLFW/include",
    },

    GLM = {
        IncludeDir = "%{wks.location}/Nut/vendor/glm",
    },

    Glad = {
        LibName = "Glad",
        IncludeDir = "%{wks.location}/Nut/vendor/Glad/include",
    },

    ImGui = {
        LibName = 'ImGui',
        IncludeDir = "%{wks.location}/Nut/vendor/imgui",
    },

    stb_image = {
        IncludeDir = "%{wks.location}/Nut/vendor/stb_image",
    },
}

function LinkDependency(table, is_debug, target)
    if table.LibDir ~= nil then
        libdirs { table.LibDir }
    end

    local libraryName = nil
    if table.LibName ~= nil then
        libraryName = table.LibName
    end 

    if table.DebugLibName ~= nil and is_debug and target == "Windows" then
        libraryName = table.DebugLibName
    end

    if libraryName ~= nil then
        links { libraryName }
        return true
    end

    return false
end

function HeaderDependency(table)
    if table.IncludeDir ~= nil then
        externalincludedirs { table.IncludeDir }
    end
end 

function IncludeDependencies(config_name)
    local target = firstToUpper(os.target())

    for key, libraryData in pairs(Dependencies) do
        local matchesConfiguration = true

        if config_name ~= nil and libraryData.Configurations ~= nil then
            matchesConfiguration = string.find(libraryData.Configurations, config_name)
        end

        local isDebug = config_name == "Debug"
        
        if matchesConfiguration then
            local continueLink = true
            HeaderDependency(libraryData)
            
            if libraryData[target] ~= nil then
                continueLink = not LinkDependency(libraryData[target], isDebug, target)
                HeaderDependency(libraryData[target])
            end

            if continueLink then
                LinkDependency(libraryData, isDebug, target)
            end
        end

    end
end


