#pragma once
#include <cstdint>

namespace Nut {

	enum class AssetFlag : uint16_t
	{
		None = 0,
		Missing = BIT(0),
		Invalid = BIT(1)
	};

	enum class AssetType : uint16_t
	{
		None = 0,
		MeshSource,
		Mesh,
		StaticMesh,
		Material,
		Texture
	};

	namespace Utils {

		inline AssetType AssetTypeFromString(std::string_view assetType)
		{
			if (assetType == "MeshSource") return AssetType::MeshSource;
			if (assetType == "Mesh") return AssetType::Mesh;
			if (assetType == "StaticMesh") return AssetType::StaticMesh;
			if (assetType == "Material") return AssetType::Material;
			if (assetType == "Texture") return AssetType::Texture;

			return AssetType::None;
		}

		inline std::string AssetTypeToString(AssetType assetType)
		{
			switch (assetType)
			{
			case AssetType::MeshSource: return "MeshSource";
			case AssetType::Mesh: return "Mesh";
			case AssetType::StaticMesh: return "StaticMesh";
			case AssetType::Material: return "Material";
			case AssetType::Texture: return "Texture";
			default: return "None";
			}
		}
	}
}