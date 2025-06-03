#include "ntpch.h"
#include "AssetImporter.h"
#include "Nut/Project/Project.h"

namespace Nut
{
	std::unordered_map<AssetType, std::unique_ptr<AssetSerializer>> AssetImporter::s_Serializers;

	void AssetImporter::Init()
	{
		s_Serializers.clear();
		s_Serializers[AssetType::Texture] = std::make_unique<TextureSerializer>();
		s_Serializers[AssetType::Material] = std::make_unique<MaterialAssetSerializer>();
	}

	void AssetImporter::Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset)
	{
		if (s_Serializers.find(metadata.Type) == s_Serializers.end())
		{
			NUT_CORE_WARN("AssetImporter::Serialize - No serializer found for asset type: {0}", metadata.FilePath.stem().string());
		}

		s_Serializers[asset->GetType()]->Serialize(metadata, asset);
	}

	void AssetImporter::Serialize(const std::shared_ptr<Asset>& asset)
	{
		const AssetMetaData& metadata = Project::GetEditorAssetManager()->GetMetaData(asset->Handle);
		Serialize(metadata, asset);
	}

	bool AssetImporter::TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset)
	{
		if (s_Serializers.find(metadata.Type) == s_Serializers.end())
		{
			NUT_CORE_WARN("AssetImporter::Serialize - No serializer found for asset type: {0}", metadata.FilePath.stem().string());
		}

		return s_Serializers[metadata.Type]->TryLoadData(metadata, asset);
	}
}


