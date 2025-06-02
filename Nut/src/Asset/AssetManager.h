#pragma once

#include "Asset.h"
#include "AssetTypes.h"
#include "Project/Project.h"

namespace Nut {

	class AssetManager
	{
	public:
		static bool IsAssetHandleValid(AssetHandle assetHandle) { return Project::GetAssetManager()->IsAssetHandleValid(assetHandle); }

		static bool ReloadData(AssetHandle assetHandle) { return Project::GetAssetManager()->ReloadData(assetHandle); }

		static AssetType GetAssetType(AssetHandle assetHandle) { return Project::GetAssetManager()->GetAssetType(assetHandle); }

		template<typename T>
		static std::shared_ptr<T> GetAsset(AssetHandle assetHandle)
		{
			std::shared_ptr<Asset> asset = Project::GetAssetManager()->GetAsset(assetHandle);
			return std::static_pointer_cast<T>(asset);
		}

		template<typename T>
		static std::unordered_set<AssetHandle> GetAllAssetsWithType()
		{
			return Project::GetAssetManager()->GetAllAssetsWithType(T::GetStaticType());
		}

		static const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetLoadedAssets() { return Project::GetAssetManager()->GetLoadedAssets(); }
		static const std::unordered_map<AssetHandle, std::shared_ptr<Asset>>& GetMemoryOnlyAssets() { return Project::GetAssetManager()->GetMemoryOnlyAssets(); }

		template<typename TAsset, typename... TArgs>
		static AssetHandle CreateMemoryOnlyAsset(TArgs&&... args)
		{
			static_assert(std::is_base_of<Asset, TAsset>::value, "CreateMemoryOnlyAsset only works for types derived from Asset");

			std::shared_ptr<TAsset> asset = std::make_shared<TAsset>(std::forward<TArgs>(args)...);
			asset->Handle = AssetHandle();

			Project::GetAssetManager()->AddMemoryOnlyAsset(asset);
			return asset->Handle;
		}

		template<typename TAsset, typename... TArgs>
		static AssetHandle CreateMemoryOnlyRendererAsset(TArgs&&... args)
		{
			static_assert(std::is_base_of<Asset, TAsset>::value, "CreateMemoryOnlyAsset only works for types derived from Asset");

			std::shared_ptr<TAsset> asset = std::make_shared<TAsset>(std::forward<TArgs>(args)...);
			asset->Handle = AssetHandle();

			Project::GetAssetManager()->AddMemoryOnlyAsset(asset);
			return asset->Handle;
		}

		template<typename TAsset, typename... TArgs>
		static AssetHandle CreateMemoryOnlyAssetWithHandle(AssetHandle handle, TArgs&&... args)
		{
			static_assert(std::is_base_of<Asset, TAsset>::value, "CreateMemoryOnlyAsset only works for types derived from Asset");

			std::shared_ptr<TAsset> asset = std::make_shared<TAsset>(std::forward<TArgs>(args)...);
			asset->Handle = handle;

			Project::GetAssetManager()->AddMemoryOnlyAsset(asset);
			return asset->Handle;
		}

		template<typename TAsset, typename... TArgs>
		static AssetHandle AddMemoryOnlyAsset(std::shared_ptr<TAsset> asset)
		{
			static_assert(std::is_base_of<Asset, TAsset>::value, "AddMemoryOnlyAsset only works for types derived from Asset");
			asset->Handle = AssetHandle();

			Project::GetAssetManager()->AddMemoryOnlyAsset(asset);
			return asset->Handle;
		}

		static bool IsMemoryAsset(AssetHandle handle) { return Project::GetAssetManager()->IsMemoryAsset(handle); }
		static void RemoveAsset(AssetHandle handle) { Project::GetAssetManager()->RemoveAsset(handle); }
	};
}