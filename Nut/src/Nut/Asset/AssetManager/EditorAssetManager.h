#pragma once

#include "AssetManagerBase.h"
#include "Nut/Asset/Asset.h"
#include "Nut/Asset/AssetMetaData.h"
#include "Nut/Asset/AssetRegistry.h"
#include "Nut/Asset/AssetImporter.h"

namespace Nut {

	class EditorAssetManager : public AssetManagerBase
	{
	public:
		EditorAssetManager();
		virtual ~EditorAssetManager();

		virtual AssetType GetAssetType(AssetHandle assetHandle) override;
		virtual std::shared_ptr<Asset> GetAsset(AssetHandle assetHandle) override;
		virtual void AddMemoryOnlyAsset(std::shared_ptr<Asset> asset) override;

		virtual std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type) override;
		virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetLoadedAssets() override;
		virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetMemoryOnlyAssets() override;

		// Only for EditorAssetManager
		const AssetMetaData& GetMetaData(AssetHandle handle);
		AssetMetaData& GetMetaDataMutable(AssetHandle handle);
		const AssetMetaData& GetMetaData(const std::filesystem::path& filepath);
		const AssetMetaData& GetMetaData(const std::shared_ptr<Asset>& asset);

		AssetHandle ImportAsset(const std::filesystem::path& filepath);

		AssetHandle GetAssetHandleFromFilePath(const std::filesystem::path& filepath);

		AssetType GetAssetTypeFromExtension(const std::string& extension);
		AssetType GetAssetTypeFromPath(const std::filesystem::path& filepath);

		std::filesystem::path GetFileSystemPath(AssetHandle handle);
		std::filesystem::path GetFileSystemPath(const AssetMetaData& metadata);
		std::string GetFileSystemPathString(const AssetMetaData& metadata);
		std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);

		bool FileExists(AssetMetaData& metadata) const;

		virtual bool ReloadData(AssetHandle assetHandle) override;
		virtual bool IsAssetHandleValid(AssetHandle assetHandle) override;
		virtual bool IsMemoryAsset(AssetHandle assetHandle) override;
		virtual bool IsAssetLoaded(AssetHandle assetHandle) override;
		virtual void RemoveAsset(AssetHandle assetHandle) override;

		const AssetRegistry& GetAssetRegistry() const { return m_AssetRegistry; }

		template<typename TAsset, typename... Args>
		std::shared_ptr<TAsset> CreateNewAsset(const std::string& filename, const std::string directory, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, TAsset>::value, "CreateNewAsset only works for types derived from Asset");

			AssetMetaData metadata;
			metadata.Handle = AssetHandle();
			if (directory.empty() || directory == ".")
				metadata.FilePath = filename;
			else
				metadata.FilePath = GetRelativePath(directory + "/" + filename);
			metadata.IsDataLoaded = true;
			metadata.Type = TAsset::GetStaticType();

			m_AssetRegistry[metadata.Handle] = metadata;

			WriteRegistryToFile();

			std::shared_ptr<TAsset> asset = std::make_shared<TAsset>(std::forward<Args>(args)...);
			asset->Handle = metadata.Handle;
			m_LoadedAssets[asset->Handle] = asset;
			//AssetImporter::Serialize(metadata, asset);

			return asset;
		}

		template<typename TAsset, typename... Args>
		AssetHandle CreateNamedMemoryOnlyAssets(const std::string& name, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, TAsset>::value, "CreateNamedMemoryOnlyAssets only works for types derived from Asset");

			std::shared_ptr<TAsset> asset = std::make_shared<TAsset>(std::forward<Args>(args)...);
			asset->Handle = AssetHandle();

			AssetMetaData metadata;
			metadata.Handle = asset->Handle;
			metadata.FilePath = name;
			metadata.IsDataLoaded = true;
			metadata.Type = TAsset::GetStaticType();
			metadata.IsMemoryOnly = true;

			m_AssetRegistry[metadata.Handle] = metadata;
			m_MemoryAssets[asset->Handle] = asset;
			return asset->Handle;
		}

		template<typename TAsset>
		std::shared_ptr<TAsset> GetAsset(const std::string& filepath)
		{
			std::shared_ptr<Asset> asset = GetAsset(GetAssetHandleFromFilePath(filepath));
			return std::dynamic_pointer_cast<TAsset>(asset);
		}

	private:
		void LoadAssetRegistry();
		void ProcessDirectory(const std::filesystem::path& directory);
		void ReloadAssets();
		void WriteRegistryToFile();

		AssetMetaData& GetMetadataInternal(AssetHandle handle);

		void OnAssetRenamed(AssetHandle assetHandle, const std::filesystem::path& newFilePath);
		void OnAssetDeleted(AssetHandle assetHandle);

	private:
		std::unordered_map<AssetHandle, std::shared_ptr<Asset>> m_LoadedAssets;
		std::unordered_map<AssetHandle, std::shared_ptr<Asset>> m_MemoryAssets;
		AssetRegistry m_AssetRegistry;
	};
}