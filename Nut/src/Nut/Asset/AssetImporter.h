#pragma once

#include "AssetSerializer.h"
#include "Nut/Scene/Scene.h"

namespace Nut {

	class AssetImporter
	{
	public:
		static void Init();
		static void Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset);
		static void Serialize(const std::shared_ptr<Asset>& asset);
		static bool TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset);

		/*static bool SerializeToAssetPack(AssetHandle handle, FileStreamWriter& stream, AssetSerializationInfo& outInfo);
		static std::shared_ptr<Asset> DeserializeFromAssetPack(FileStreamReader& stream, const AssetPackFile::AssetInfo& assetInfo);
		static std::shared_ptr<Scene> DeserializeSceneFromAssetPack(FileStreamReader& stream, const AssetPackFile::SceneInfo& assetInfo);*/

	private:
		static std::unordered_map<AssetType, std::unique_ptr<AssetSerializer>> s_Serializers;
	};
}