#pragma once

#include "AssetManagerBase.h"

namespace Nut {

	class RuntimeAssetManager : public AssetManagerBase
	{
	public:
		RuntimeAssetManager();
		virtual ~RuntimeAssetManager();

		virtual AssetType GetAssetType(AssetHandle assetHandle) override;
		virtual std::shared_ptr<Asset> GetAsset(AssetHandle assetHandle) override;
		virtual void AddMemoryOnlyAsset(std::shared_ptr<Asset> asset) override;
		virtual bool ReloadData(AssetHandle assetHandle) override;
		virtual bool IsAssetHandleValid(AssetHandle assetHandle) override;
		virtual bool IsMemoryAsset(AssetHandle assetHandle) override;
		virtual bool IsAssetLoaded(AssetHandle assetHandle) override;
		virtual void RemoveAsset(AssetHandle assetHandle) override;

		virtual std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type) override;
		virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetLoadedAssets() override { return m_LoadedAssets; }
		virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetMemoryOnlyAssets() override { return m_MemoryAssets; }

	private:
		std::unordered_map<AssetHandle, std::shared_ptr<Asset>> m_LoadedAssets;
		std::unordered_map<AssetHandle, std::shared_ptr<Asset>> m_MemoryAssets;
	};
}