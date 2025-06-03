#include "ntpch.h"
#include "RuntimeAssetManager.h"

namespace Nut
{
	// TODO: complete the implementation of RuntimeAssetManager
	RuntimeAssetManager::RuntimeAssetManager()
	{
	}

	RuntimeAssetManager::~RuntimeAssetManager()
	{
	}
	AssetType RuntimeAssetManager::GetAssetType(AssetHandle assetHandle)
	{
		return AssetType();
	}
	std::shared_ptr<Asset> RuntimeAssetManager::GetAsset(AssetHandle assetHandle)
	{
		return std::shared_ptr<Asset>();
	}
	void RuntimeAssetManager::AddMemoryOnlyAsset(std::shared_ptr<Asset> asset)
	{
	}
	bool RuntimeAssetManager::ReloadData(AssetHandle assetHandle)
	{
		return false;
	}
	bool RuntimeAssetManager::IsAssetHandleValid(AssetHandle assetHandle)
	{
		return false;
	}
	bool RuntimeAssetManager::IsMemoryAsset(AssetHandle assetHandle)
	{
		return false;
	}
	bool RuntimeAssetManager::IsAssetLoaded(AssetHandle assetHandle)
	{
		return false;
	}
	void RuntimeAssetManager::RemoveAsset(AssetHandle assetHandle)
	{
	}
	std::unordered_set<AssetHandle> RuntimeAssetManager::GetAllAssetsWithType(AssetType type)
	{
		return std::unordered_set<AssetHandle>();
	}
}
