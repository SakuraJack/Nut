#pragma once

#include "glm/glm.hpp"
#include "Shader.h"

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
		unsigned int m_BaseVertex;
		unsigned int m_BaseIndex;
		unsigned int m_VertexCount;
		unsigned int m_IndexCount;

		glm::mat4 m_LocalTransform; // 局部变换矩阵
		glm::mat4 m_GlobalTransform; // 全局变换矩阵
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<SubMesh>& subMeshes)
			: m_Vertices(vertices), m_Indices(indices), m_SubMeshes(subMeshes) {}
		~Mesh();

		void SetVertices(const std::vector<Vertex>& vertices) { m_Vertices = vertices; }
		void SetIndices(const std::vector<Index>& indices) { m_Indices = indices; }
		void SetSubMeshes(const std::vector<SubMesh>& subMeshes) { m_SubMeshes = subMeshes; }

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<Index>& GetIndices() const { return m_Indices; }
		const std::vector<SubMesh>& GetSubMeshes() const { return m_SubMeshes; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;
		std::vector<SubMesh> m_SubMeshes;
	};
}