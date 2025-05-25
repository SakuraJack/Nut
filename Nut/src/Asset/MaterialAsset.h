#pragma once

#include "Asset.h"
#include "Renderer/Material.h"

namespace Nut {

	class MaterialAsset : public Asset
	{
	public:
		MaterialAsset(bool transparent = false);
		MaterialAsset(std::shared_ptr<Material> material);
		~MaterialAsset();

		glm::vec3& GetAlbedoColor();
		void SetAlbedoColor(const glm::vec3& color);

		float& GetMetalness();
		void SetMetalness(float value);

		float& GetRoughness();
		void SetRoughness(float value);

		float& GetEmission();
		void SetEmission(float value);

		std::shared_ptr<Texture2D> GetAlbedoMap();
		void SetAlbedoMap(std::shared_ptr<Texture2D> texture);
		void ClearAlbedoMap();

		std::shared_ptr<Texture2D> GetNormalMap();
		void SetNormalMap(std::shared_ptr<Texture2D> texture);
		bool UseNormalMap();
		void SetUseNormalMap(bool use);
		void ClearNormalMap();

		std::shared_ptr<Texture2D> GetMetalnessMap();
		void SetMetalnessMap(std::shared_ptr<Texture2D> texture);
		void ClearMetalnessMap();

		std::shared_ptr<Texture2D> GetRoughnessMap();
		void SetRoughnessMap(std::shared_ptr<Texture2D> texture);
		void ClearRoughnessMap();

		float& GetTransparency();
		void SetTransparency(float value);

		// TODO: “ı”∞
		// bool IsShadowCasting() const {}
		// void SetShadowCasting(bool cast) { m_CastShadow = cast; }

		static AssetType GetStaticType() { return AssetType::Material; }
		virtual AssetType GetType() const override { return GetStaticType(); }

		std::shared_ptr<Material> GetMaterial() const { return m_Material; }
		void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

		bool IsTransparent() const { return m_Transparent; }

	private:
		std::shared_ptr<Material> m_Material;
		bool m_Transparent = false;
	};

	class MaterialTable
	{
	public:
		MaterialTable(uint32_t materialCount = 1);
		MaterialTable(std::shared_ptr<MaterialTable> other);
		~MaterialTable() = default;

		bool HasMaterial(uint32_t materialIndex) const { return m_Materials.find(materialIndex) != m_Materials.end(); }
		void SetMaterial(uint32_t materialIndex, AssetHandle material);
		void ClearMaterial(uint32_t materialIndex);

		AssetHandle GetMaterial(uint32_t materialIndex) const;

		std::map<uint32_t, AssetHandle>& GetMaterials() { return m_Materials; }
		const std::map<uint32_t, AssetHandle>& GetMaterials() const { return m_Materials; }

		uint32_t GetMaterialCount() const { return m_MaterialCount; }
		void SetMaterialCount(uint32_t count) { m_MaterialCount = count; }

		void Clear();

	private:
		std::map<uint32_t, AssetHandle> m_Materials;
		uint32_t m_MaterialCount = 0;
	};
}