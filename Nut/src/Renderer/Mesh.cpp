#include "ntpch.h"
#include "Mesh.h"
#include <Asset/MaterialAsset.h>

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

Nut::Mesh::Mesh(std::shared_ptr<MeshSource> meshSource)
	: m_MeshSource(meshSource)
{
	Handle = {};

	SetSubmeshes({});

	const auto& meshMaterials = meshSource->GetMaterials();
	m_Materials = std::make_shared<Nut::MaterialTable>((uint32_t)meshMaterials.size());
	for (size_t i = 0; i < meshMaterials.size(); ++i) {
		std::shared_ptr<MaterialAsset> m = std::make_shared<MaterialAsset>(meshMaterials[i]);
		m->Handle = AssetHandle();
		m_MaterialAssets.push_back(m);
		m_Materials->SetMaterial((uint32_t)i, m->Handle);
	}
}

Nut::Mesh::Mesh(std::shared_ptr<MeshSource> meshSource, const std::vector<uint32_t>& submeshes)
	: m_MeshSource(meshSource)
{
	Handle = {};

	SetSubmeshes(submeshes);

	const auto& meshMaterials = meshSource->GetMaterials();
	m_Materials = std::make_shared<Nut::MaterialTable>((uint32_t)meshMaterials.size());
	for (size_t i = 0; i < meshMaterials.size(); ++i) {
		std::shared_ptr<MaterialAsset> m = std::make_shared<MaterialAsset>(meshMaterials[i]);
		m->Handle = AssetHandle();
		m_MaterialAssets.push_back(m);
		m_Materials->SetMaterial((uint32_t)i, m->Handle);
	}
}

Nut::Mesh::Mesh(const std::shared_ptr<Mesh>& other)
	: m_MeshSource(other->m_MeshSource)
{
	SetSubmeshes(other->m_Submeshes);
}

Nut::Mesh::~Mesh()
{

}

void Nut::Mesh::SetSubmeshes(const std::vector<uint32_t>& submeshes)
{
	if (!submeshes.empty()) {
		m_Submeshes = submeshes;
	}
	else {
		const auto& submeshes = m_MeshSource->GetSubmeshes();
		m_Submeshes.resize(submeshes.size());
		for (uint32_t i = 0; i < submeshes.size(); i++) {
			m_Submeshes[i] = i;
		}
	}
}

Nut::StaticMesh::StaticMesh(std::shared_ptr<MeshSource> meshSource)
	: m_MeshSource(meshSource)
{
	Handle = {};

	SetSubmeshes({});

	const auto& meshMaterials = meshSource->GetMaterials();
	m_Materials = std::make_shared<Nut::MaterialTable>((uint32_t)meshMaterials.size());
	for (size_t i = 0; i < meshMaterials.size(); ++i) {
		std::shared_ptr<MaterialAsset> m = std::make_shared<MaterialAsset>(meshMaterials[i]);
		m->Handle = AssetHandle();
		m_MaterialAssets.push_back(m);
		m_Materials->SetMaterial((uint32_t)i, m->Handle);
	}
}

Nut::StaticMesh::StaticMesh(std::shared_ptr<MeshSource> meshSource, const std::vector<uint32_t>& submeshes)
	: m_MeshSource(meshSource)
{
	Handle = {};

	SetSubmeshes(submeshes);

	const auto& meshMaterials = meshSource->GetMaterials();
	m_Materials = std::make_shared<Nut::MaterialTable>((uint32_t)meshMaterials.size());
	for (size_t i = 0; i < meshMaterials.size(); ++i) {
		std::shared_ptr<MaterialAsset> m = std::make_shared<MaterialAsset>(meshMaterials[i]);
		m->Handle = AssetHandle();
		m_MaterialAssets.push_back(m);
		m_Materials->SetMaterial((uint32_t)i, m->Handle);
	}
}

Nut::StaticMesh::StaticMesh(const std::shared_ptr<StaticMesh>& other)
	: m_MeshSource(other->m_MeshSource)
{
	SetSubmeshes(other->m_Submeshes);
}

Nut::StaticMesh::~StaticMesh()
{

}

void Nut::StaticMesh::SetSubmeshes(const std::vector<uint32_t>& submeshes)
{
	if (!submeshes.empty()) {
		m_Submeshes = submeshes;
	}
	else {
		const auto& submeshes = m_MeshSource->GetSubmeshes();
		m_Submeshes.resize(submeshes.size());
		for (uint32_t i = 0; i < submeshes.size(); i++) {
			m_Submeshes[i] = i;
		}
	}
}