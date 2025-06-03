#include "ntpch.h"
#include "AssetSerializer.h"

void Nut::TextureSerializer::Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const
{
}

bool Nut::TextureSerializer::TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const
{
	return false;
}

void Nut::MaterialAssetSerializer::Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const
{
}

bool Nut::MaterialAssetSerializer::TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const
{
	return false;
}

void Nut::SceneAssetSerializer::Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const
{
}

bool Nut::SceneAssetSerializer::TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const
{
	return false;
}
