#pragma once

#include "glm/glm.hpp"
#include "Asset/Asset.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Material.h"
#include "Asset/MaterialAsset.h"

namespace Nut {
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Index
	{
		uint32_t V1, V2, V3;
	};

	struct Triangle
	{
		Vertex V0, V1, V2;

		Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
			: V0(v0), V1(v1), V2(v2) {}
	};

	class SubMesh
	{
	public:
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t VertexCount;
		uint32_t IndexCount;

		glm::mat4 m_LocalTransform = glm::mat4(1.f); // 局部变换矩阵
		glm::mat4 m_GlobalTransform = glm::mat4(1.f); // 全局变换矩阵
	};

	class MeshSource : public Asset
	{
	public:
		MeshSource() = default;
		MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
		MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& submeshes);
		virtual ~MeshSource();

		void DumpVertex();

		std::vector<SubMesh>& GetSubmeshes() { return m_Submeshes; }
		const std::vector<SubMesh>& GetSubmeshes() const { return m_Submeshes; }

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<Index>& GetIndices() const { return m_Indices; }
		std::vector<std::shared_ptr<Material>> GetMaterials() { return m_Materials; }
		const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return m_Materials; }
		const std::string& GetFilePath() const { return m_FilePath; }

		std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
		std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }
		std::shared_ptr<VertexArray> GetVertexArray() const { return m_VertexArray; }

		static AssetType GetStaticType() { return AssetType::MeshSource; }
		AssetType GetType() const override { return GetStaticType(); }

	private:
		std::vector<SubMesh> m_Submeshes;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::vector<std::shared_ptr<Material>> m_Materials;

		std::string m_FilePath;

		friend class GeometryGenerator;
	};

	class Mesh : public Asset // 动态网格
	{
	public:
		explicit Mesh(std::shared_ptr<MeshSource> meshSource);
		Mesh(std::shared_ptr<MeshSource> meshSource, const std::vector<uint32_t>& submeshes);
		Mesh(const std::shared_ptr<Mesh>& other);
		virtual ~Mesh();

		void SetSubmeshes(const std::vector<uint32_t>& submeshes);
		std::vector<uint32_t>& GetSubmeshes() { return m_Submeshes; }
		const std::vector<uint32_t>& GetSubmeshes() const { return m_Submeshes; }
		
		std::shared_ptr<MeshSource> GetMeshSource() { return m_MeshSource; }
		const std::shared_ptr<MeshSource> GetMeshSource() const { return m_MeshSource; }
		void SetMeshSource(std::shared_ptr<MeshSource> meshSource) { m_MeshSource = meshSource; }

		std::shared_ptr<MaterialTable> GetMaterials() const { return m_Materials; }

		static AssetType GetStaticType() { return AssetType::Mesh; }
		AssetType GetType() const override { return GetStaticType(); }

		// TODO: 放入AssetManager由每个项目管理
		std::vector<std::shared_ptr<MaterialAsset>> m_MaterialAssets; 

	private:
		std::vector<uint32_t> m_Submeshes;
		std::shared_ptr<MeshSource> m_MeshSource;
		std::shared_ptr<MaterialTable> m_Materials;

		friend class Material;
		friend class Renderer;
		friend class RendererAPI;
	};

	class StaticMesh : public Asset // 静态网格
	{
	public:
		explicit StaticMesh(std::shared_ptr<MeshSource> meshSource);
		StaticMesh(std::shared_ptr<MeshSource> meshSource, const std::vector<uint32_t>& submeshes);
		StaticMesh(const std::shared_ptr<StaticMesh>& other);
		virtual ~StaticMesh();

		void SetSubmeshes(const std::vector<uint32_t>& submeshes);
		std::vector<uint32_t>& GetSubmeshes() { return m_Submeshes; }
		const std::vector<uint32_t>& GetSubmeshes() const { return m_Submeshes; }

		std::shared_ptr<MeshSource> GetMeshSource() { return m_MeshSource; }
		const std::shared_ptr<MeshSource> GetMeshSource() const { return m_MeshSource; }
		void SetMeshSource(std::shared_ptr<MeshSource> meshSource) { m_MeshSource = meshSource; }

		std::shared_ptr<MaterialTable> GetMaterials() const { return m_Materials; }

		static AssetType GetStaticType() { return AssetType::StaticMesh; }
		AssetType GetType() const override { return GetStaticType(); }

		// TODO: 放入AssetManager由每个项目管理
		std::vector<std::shared_ptr<MaterialAsset>> m_MaterialAssets;

	private:
		std::vector<uint32_t> m_Submeshes;
		std::shared_ptr<MeshSource> m_MeshSource;
		std::shared_ptr<MaterialTable> m_Materials;

		friend class Material;
		friend class Renderer;
		friend class RendererAPI;
	};
}