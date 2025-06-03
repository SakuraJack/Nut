#pragma once

#include "Nut/Asset/Asset.h"
#include "Nut/Asset/AssetTypes.h"

#include <unordered_set>
#include <unordered_map>
#include <memory>

namespace Nut {

	class AssetManagerBase
	{
	public:
		AssetManagerBase() = default;
		virtual ~AssetManagerBase() = default;

		virtual AssetType GetAssetType(AssetHandle assetHandle) = 0;
		virtual std::shared_ptr<Asset> GetAsset(AssetHandle assetHandle) = 0;
		virtual void AddMemoryOnlyAsset(std::shared_ptr<Asset> asset) = 0;
		virtual bool ReloadData(AssetHandle assetHandle) = 0;
		virtual bool IsAssetHandleValid(AssetHandle assetHandle) = 0;
		virtual bool IsMemoryAsset(AssetHandle assetHandle) = 0;
		virtual bool IsAssetLoaded(AssetHandle assetHandle) = 0;
		virtual void RemoveAsset(AssetHandle assetHandle) = 0;

		virtual std::unordered_set<AssetHandle> GetAllAssetsWithType(AssetType type) = 0;
		virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetLoadedAssets() = 0;
		virtual const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetMemoryOnlyAssets() = 0;
	};
}