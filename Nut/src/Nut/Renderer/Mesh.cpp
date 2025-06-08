#include "ntpch.h"
#include "Mesh.h"
#include <Nut/Asset/MaterialAsset.h>
#include "Nut/Asset/AssetManager.h"

Nut::MeshSource::MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform)
	: m_Vertices(vertices), m_Indices(indices)
{
	Handle = {};

	SubMesh submesh;
	submesh.BaseVertex = 0;
	submesh.BaseIndex = 0;
	submesh.MaterialIndex = 0;
	submesh.IndexCount = (uint32_t)indices.size() * 3u;
	submesh.m_GlobalTransform = transform;
	m_Submeshes.push_back(submesh);

	m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), (uint64_t)(m_Vertices.size() * sizeof(Vertex)));
	m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint64_t)(m_Indices.size() * sizeof(Index)));
	m_VertexBuffer->SetLayout({
		{"a_Position", DataType::Float3},
		{"a_Normal", DataType::Float3},
		{"a_TexCoord", DataType::Float2},
		{"a_Tangent", DataType::Float3},
		{"a_Bitangent", DataType::Float3} });
	m_VertexArray = VertexArray::Create(m_VertexBuffer, m_IndexBuffer);
}

Nut::MeshSource::MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& submeshes)
	: m_Vertices(vertices), m_Indices(indices), m_Submeshes(submeshes)
{
	Handle = {};

	m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), (uint64_t)(m_Vertices.size() * sizeof(Vertex)));
	m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint64_t)(m_Indices.size() * sizeof(Index)));
	m_VertexBuffer->SetLayout({
		{"a_Position", DataType::Float3},
		{"a_Normal", DataType::Float3},
		{"a_TexCoord", DataType::Float2},
		{"a_Tangent", DataType::Float3},
		{"a_Bitangent", DataType::Float3} });
	m_VertexArray = VertexArray::Create(m_VertexBuffer, m_IndexBuffer);
}

Nut::MeshSource::~MeshSource()
{

}

void Nut::MeshSource::DumpVertex()
{

}

Nut::Mesh::Mesh(AssetHandle meshSource)
	: m_MeshSource(meshSource)
{
	Handle = {};

	m_Materials = std::make_shared<Nut::MaterialTable>();

	if (auto meshSourceAsset = AssetManager::GetAsset<MeshSource>(meshSource); meshSourceAsset)
	{
		SetSubmeshes({}, meshSourceAsset);

		const std::vector<AssetHandle>& meshMaterials = meshSourceAsset->GetMaterials();
		for (size_t i = 0; i < meshMaterials.size(); ++i) {
			m_Materials->SetMaterial((uint32_t)i, meshMaterials[i]);
		}
	}
}

Nut::Mesh::Mesh(AssetHandle meshSource, const std::vector<uint32_t>& submeshes)
	: m_MeshSource(meshSource)
{
	Handle = {};

	m_Materials = std::make_shared<MaterialTable>();

	if (auto meshSourceAsset = AssetManager::GetAsset<MeshSource>(meshSource); meshSourceAsset)
	{
		SetSubmeshes(submeshes, meshSourceAsset);

		const std::vector<AssetHandle>& meshMaterials = meshSourceAsset->GetMaterials();
		for (size_t i = 0; i < meshMaterials.size(); ++i) {
			m_Materials->SetMaterial((uint32_t)i, meshMaterials[i]);
		}
	}
}

Nut::Mesh::~Mesh()
{

}

void Nut::Mesh::SetSubmeshes(const std::vector<uint32_t>& submeshes, std::shared_ptr<MeshSource> meshSource)
{
	if (!submeshes.empty()) {
		m_Submeshes = submeshes;
	}
	else {
		const auto& submeshes = meshSource->GetSubmeshes();
		m_Submeshes.resize(submeshes.size());
		for (uint32_t i = 0; i < submeshes.size(); i++) {
			m_Submeshes[i] = i;
		}
	}
}

Nut::StaticMesh::StaticMesh(AssetHandle meshSource)
	: m_MeshSource(meshSource)
{
	Handle = {};

	m_Materials = std::make_shared<MaterialTable>();

	if (auto meshSourceAsset = AssetManager::GetAsset<MeshSource>(meshSource); meshSourceAsset)
	{
		SetSubmeshes({}, meshSourceAsset);

		const std::vector<AssetHandle>& meshMaterials = meshSourceAsset->GetMaterials();
		for (size_t i = 0; i < meshMaterials.size(); ++i) {
			m_Materials->SetMaterial((uint32_t)i, meshMaterials[i]);
		}
	}
}

Nut::StaticMesh::StaticMesh(AssetHandle meshSource, const std::vector<uint32_t>& submeshes)
	: m_MeshSource(meshSource)
{
	Handle = {};

	m_Materials = std::make_shared<MaterialTable>();

	if (auto meshSourceAsset = AssetManager::GetAsset<MeshSource>(meshSource); meshSourceAsset)
	{
		SetSubmeshes(submeshes, meshSourceAsset);

		const std::vector<AssetHandle>& meshMaterials = meshSourceAsset->GetMaterials();
		for (size_t i = 0; i < meshMaterials.size(); ++i) {
			m_Materials->SetMaterial((uint32_t)i, meshMaterials[i]);
		}
	}
}

Nut::StaticMesh::~StaticMesh()
{

}

void Nut::StaticMesh::SetSubmeshes(const std::vector<uint32_t>& submeshes, std::shared_ptr<MeshSource> meshSource)
{
	if (!submeshes.empty()) {
		m_Submeshes = submeshes;
	}
	else {
		const auto& submeshes = meshSource->GetSubmeshes();
		m_Submeshes.resize(submeshes.size());
		for (uint32_t i = 0; i < submeshes.size(); i++) {
			m_Submeshes[i] = i;
		}
	}
}