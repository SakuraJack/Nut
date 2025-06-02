#include "ntpch.h"
#include "EditorAssetManager.h"

#include "Project/Project.h"

namespace Nut
{
	static AssetMetaData s_NullMetadata;

	EditorAssetManager::EditorAssetManager()
	{
		AssetImporter::Init();

		LoadAssetRegistry();
		ReloadAssets();
	}

	EditorAssetManager::~EditorAssetManager()
	{
		WriteRegistryToFile();
	}

	AssetType EditorAssetManager::GetAssetType(AssetHandle assetHandle)
	{
		if (!IsAssetHandleValid(assetHandle)) {
			return AssetType::None;
		}

		const auto& metadata = GetMetaData(assetHandle);
		return metadata.Type;
	}

	std::shared_ptr<Asset> EditorAssetManager::GetAsset(AssetHandle assetHandle)
	{
		if (IsMemoryAsset(assetHandle)) {
			return m_MemoryAssets[assetHandle];
		}

		auto& metadata = GetMetadataInternal(assetHandle);
		if (!metadata.IsValid())
			return nullptr;

		std::shared_ptr<Asset> asset = nullptr;
		if (!metadata.IsDataLoaded) {
			metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
			if (!metadata.IsDataLoaded)
				return nullptr;

			m_LoadedAssets[assetHandle] = asset;
		}
		else {
			asset = m_LoadedAssets[assetHandle];
		}

		return asset;
	}

	void EditorAssetManager::AddMemoryOnlyAsset(std::shared_ptr<Asset> asset)
	{
		AssetMetaData metadata;
		metadata.Handle = asset->Handle;
		metadata.IsDataLoaded = true;
		metadata.Type = asset->GetType();
		metadata.IsMemoryOnly = true;
		m_AssetRegistry[metadata.Handle] = metadata;
		m_MemoryAssets[asset->Handle] = asset;
	}

	std::unordered_set<AssetHandle> EditorAssetManager::GetAllAssetsWithType(AssetType type)
	{
		std::unordered_set<AssetHandle> result;
		for (const auto& [handle, metadata] : m_AssetRegistry) {
			if (metadata.Type == type) {
				result.insert(handle);
			}
		}
		return result;
	}

	const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& EditorAssetManager::GetLoadedAssets()
	{
		return m_LoadedAssets;
	}

	const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& EditorAssetManager::GetMemoryOnlyAssets()
	{
		return m_MemoryAssets;
	}

	const AssetMetaData& EditorAssetManager::GetMetaData(AssetHandle handle)
	{
		if (m_AssetRegistry.Contains(handle))
			return m_AssetRegistry[handle];
		return s_NullMetadata;
	}

	AssetMetaData& EditorAssetManager::GetMetaDataMutable(AssetHandle handle)
	{
		if (m_AssetRegistry.Contains(handle))
			return m_AssetRegistry[handle];
		return s_NullMetadata;
	}

	const AssetMetaData& EditorAssetManager::GetMetaData(const std::filesystem::path& filepath)
	{
		const auto& relativePath = GetRelativePath(filepath);

		for (auto& [handle, metadata] : m_AssetRegistry) {
			if (metadata.FilePath == relativePath)
				return metadata;
		}
		return s_NullMetadata;
	}

	const AssetMetaData& EditorAssetManager::GetMetaData(const std::shared_ptr<Asset>& asset)
	{
		return GetMetaData(asset->Handle);
	}

	AssetHandle EditorAssetManager::ImportAsset(const std::filesystem::path& filepath)
	{
		std::filesystem::path path = GetRelativePath(filepath);

		if (auto& metadata = GetMetaData(path); metadata.IsValid())
			return metadata.Handle;

		AssetType type = GetAssetTypeFromPath(path);
		if (type == AssetType::None)
			return 0;

		AssetMetaData metadata;
		metadata.Handle = AssetHandle();
		metadata.FilePath = path;
		metadata.Type = type;
		m_AssetRegistry[metadata.Handle] = metadata;

		return metadata.Handle;
	}

	AssetHandle EditorAssetManager::GetAssetHandleFromFilePath(const std::filesystem::path& filepath)
	{
		return GetMetaData(filepath).Handle;
	}

	AssetType EditorAssetManager::GetAssetTypeFromExtension(const std::string& extension)
	{
		// TODO: fill this
		return AssetType::None;
	}

	AssetType EditorAssetManager::GetAssetTypeFromPath(const std::filesystem::path& filepath)
	{
		return GetAssetTypeFromExtension(filepath.extension().string());
	}

	std::filesystem::path EditorAssetManager::GetFileSystemPath(AssetHandle handle)
	{
		return GetFileSystemPathString(GetMetaData(handle));
	}

	std::filesystem::path EditorAssetManager::GetFileSystemPath(const AssetMetaData& metadata)
	{
		return Project::GetAssetDirectory() / metadata.FilePath;
	}

	std::string EditorAssetManager::GetFileSystemPathString(const AssetMetaData& metadata)
	{
		return GetFileSystemPath(metadata).string();
	}

	std::filesystem::path EditorAssetManager::GetRelativePath(const std::filesystem::path& filepath)
	{
		std::filesystem::path relativePath = filepath.lexically_normal();
		std::string temp = filepath.string();
		if (temp.find(Project::GetAssetDirectory().string()) != std::string::npos) {
			relativePath = std::filesystem::relative(filepath, Project::GetAssetDirectory());
			if (relativePath.empty())
				relativePath = filepath.lexically_normal();
		}
		return relativePath;
	}

	bool EditorAssetManager::FileExists(AssetMetaData& metadata) const
	{
		return std::filesystem::exists(Project::GetActive()->GetAssetDirectory() / metadata.FilePath);
	}

	bool EditorAssetManager::ReloadData(AssetHandle assetHandle)
	{
		auto& metadata = GetMetadataInternal(assetHandle);
		if (!metadata.IsValid())
			return false;

		std::shared_ptr<Asset> asset;
		metadata.IsDataLoaded = AssetImporter::TryLoadData(metadata, asset);
		if (metadata.IsDataLoaded)
			m_LoadedAssets[assetHandle] = asset;

		return metadata.IsDataLoaded;
	}

	bool EditorAssetManager::IsAssetHandleValid(AssetHandle assetHandle)
	{
		return IsMemoryAsset(assetHandle) || GetMetaData(assetHandle).IsValid();
	}

	bool EditorAssetManager::IsMemoryAsset(AssetHandle assetHandle)
	{
		return m_MemoryAssets.find(assetHandle) != m_MemoryAssets.end();
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle assetHandle)
	{
		return m_LoadedAssets.contains(assetHandle);
	}

	void EditorAssetManager::RemoveAsset(AssetHandle assetHandle)
	{
		if (m_LoadedAssets.contains(assetHandle))
			m_LoadedAssets.erase(assetHandle);

		if (m_MemoryAssets.contains(assetHandle))
			m_MemoryAssets.erase(assetHandle);

		if (m_AssetRegistry.Contains(assetHandle))
			m_AssetRegistry.Remove(assetHandle);
	}

	void EditorAssetManager::LoadAssetRegistry()
	{
		// TODO: Implement loading the asset registry from a file.
		//const auto& assetRegistryPath = Project::GetAssetRegistryPath();
		//if (!std::filesystem::exists(assetRegistryPath))
		//	return;

		//std::ifstream stream(assetRegistryPath);
		//std::stringstream strStream;
		//strStream << stream.rdbuf();

		//// TODO: use YAML or JSON ?
		//YAML::Node data = YAML::Load(strStream.str());
		//auto handles = data["Assets"];
		//if (!handles)
		//	return;

		//for (auto entry : handles) {

		//}
	}

	void EditorAssetManager::ProcessDirectory(const std::filesystem::path& directory)
	{
		// TODO: Implement processing the directory to find assets.
	}

	void EditorAssetManager::ReloadAssets()
	{
		// TODO: Implement reloading all assets in the project.
	}

	void EditorAssetManager::WriteRegistryToFile()
	{
		// TODO: Implement writing the asset registry to a file.
	}

	AssetMetaData& EditorAssetManager::GetMetadataInternal(AssetHandle handle)
	{
		if (m_AssetRegistry.Contains(handle))
			return m_AssetRegistry[handle];
		return s_NullMetadata;
	}

	void EditorAssetManager::OnAssetRenamed(AssetHandle assetHandle, const std::filesystem::path& newFilePath)
	{
		// TODO: Implement handling asset renaming.
	}

	void EditorAssetManager::OnAssetDeleted(AssetHandle assetHandle)
	{

		// TODO: Implement handling asset deletion.
	}

}
