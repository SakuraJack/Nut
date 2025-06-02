#pragma once

#include "Asset/AssetManager/AssetManagerBase.h"
#include "Asset/AssetManager/EditorAssetManager.h"
#include "Asset/AssetManager/RuntimeAssetManager.h"

namespace Nut {

	struct ProjectConfig
	{
		std::string Name;

		std::string AssetDirectory;
		std::string AssetRegistryPath;

		std::string MeshPath;
		std::string MeshSourcePath;

		std::string StartScene;

		bool EnableAutoSave = false;
		int AutoSaveIntervalSeconds = 300;

		std::string ProjectFileName;
		std::string ProjectDirectory;
	};

	class Project
	{
	public:
		Project();
		~Project();

		const ProjectConfig& GetConfig() const { return m_Config; }

		static std::shared_ptr<Project> GetActive() { return s_ActiveProject; }
		static void SetActive(const std::shared_ptr<Project>& project);

		inline static std::shared_ptr<AssetManagerBase> GetAssetManager() { return s_AssetManager;  }
		inline static std::shared_ptr<EditorAssetManager> GetEditorAssetManager() { return std::dynamic_pointer_cast<EditorAssetManager>(s_AssetManager); }
		inline static std::shared_ptr<RuntimeAssetManager> GetRuntimeAssetManager() { return std::dynamic_pointer_cast<RuntimeAssetManager>(s_AssetManager); }

		static const std::string& GetProjectName()
		{
			return s_ActiveProject->GetConfig().Name;
		}

		static std::filesystem::path GetProjectDirectory()
		{
			return s_ActiveProject->GetConfig().ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / s_ActiveProject->GetConfig().AssetDirectory;
		}

		static std::filesystem::path GetAssetRegistryPath()
		{
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / s_ActiveProject->GetConfig().AssetRegistryPath;
		}

		static std::filesystem::path GetMeshPath()
		{
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / s_ActiveProject->GetConfig().MeshPath;
		}

		static std::filesystem::path GetCacheDirectory()
		{
			return std::filesystem::path(s_ActiveProject->GetConfig().ProjectDirectory) / "Cache";
		}

	private:
		ProjectConfig m_Config;
		inline static std::shared_ptr<AssetManagerBase> s_AssetManager;

		inline static std::shared_ptr<Project> s_ActiveProject;
	};
}