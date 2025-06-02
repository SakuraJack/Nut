#pragma once

#include "AssetMetaData.h"

namespace Nut {

	struct AssetSerializationInfo
	{
		uint64_t Offset = 0;
		uint64_t Size = 0;
	};

	class AssetSerializer
	{
	public:
		virtual void Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const = 0;
		virtual bool TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const = 0;
	};

	class TextureSerializer : public AssetSerializer
	{
		virtual void Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const override;
		virtual bool TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const override;
	};

	class MaterialAssetSerializer : public AssetSerializer
	{
		virtual void Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const override;
		virtual bool TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const override;
	};

	class SceneAssetSerializer : public AssetSerializer
	{
		virtual void Serialize(const AssetMetaData& metadata, const std::shared_ptr<Asset>& asset) const override;
		virtual bool TryLoadData(const AssetMetaData& metadata, std::shared_ptr<Asset>& asset) const override;
	};
}