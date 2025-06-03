#include "ntpch.h"
#include "SimpleGeometryGenerator.h"

#include <glm/ext/scalar_constants.hpp>

std::shared_ptr<Nut::MeshSource> Nut::GeometryGenerator::CreateCube(const glm::vec3& pos /*= glm::vec3(0.f)*/, float sideLength /*= 1.0f*/)
{
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.f, 1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.f, 0.f, -1.f), glm::vec2(1.f, 1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
		Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
	};

	std::vector<SubMesh> submeshes = {
		SubMesh{0, 0, 0, 8, 12, glm::mat4(1.0f), glm::mat4(1.0f)}
	};

	std::vector<Index> indices = {
		// Front face
		Index(0, 1, 2),
		Index(1, 3, 2),

		// Back face
		Index(4, 6, 5),
		Index(6, 7, 5),

		// Left face
		Index(0, 4, 1),
		Index(1, 4, 5),

		// Right face
		Index(2, 3, 6),
		Index(3, 7, 6),

		// Top face
		Index(1, 5, 3),
		Index(3, 5, 7),

		// Bottom face
		Index(0, 2, 4),
		Index(2, 6, 4),
	};

	for (auto& vertex : vertices) {
		vertex.Position *= sideLength;
		vertex.Position += pos;
	}

	std::shared_ptr<MeshSource> meshSource = std::make_shared<MeshSource>(vertices, indices, submeshes);
	meshSource->m_Materials.push_back(Material::Create(Shader::Create()));

	return meshSource;
}

std::shared_ptr<Nut::MeshSource> Nut::GeometryGenerator::CreateSphere(const glm::vec3& pos /*= glm::vec3(0.f)*/, float radius /*= 1.0f*/, int sliceCount /*= 20*/, int stackCount /*= 20*/)
{
	std::vector<Vertex> vertices;
	std::vector<SubMesh> submeshes;
	std::vector<Index> indices;

	// Generate vertices
	for (int i = 0; i <= stackCount; ++i) {
		float phi = glm::pi<float>() * i / stackCount;
		for (int j = 0; j <= sliceCount; ++j) {
			float theta = 2.0f * glm::pi<float>() * j / sliceCount;

			glm::vec3 position(
				radius * sinf(phi) * cosf(theta),
				radius * cosf(phi),
				radius * sinf(phi) * sinf(theta)
			);

			glm::vec3 normal = glm::normalize(position);
			glm::vec2 texCoord(static_cast<float>(j) / sliceCount, static_cast<float>(i) / stackCount);

			vertices.push_back(Vertex(position, normal, texCoord, glm::vec3(1.f), glm::vec3(0.f)));
		}
	}

	// Generate indices
	for (int i = 0; i < stackCount; ++i) {
		for (int j = 0; j < sliceCount; ++j) {
			int first = i * (sliceCount + 1) + j;
			int second = first + sliceCount + 1;

			indices.push_back(Index(first, second, first + 1));
			indices.push_back(Index(second, second + 1, first + 1));
		}
	}

	submeshes.push_back(SubMesh{ 0, 0, 0, static_cast<uint32_t>(vertices.size()), static_cast<uint32_t>(indices.size()), glm::mat4(1.0f), glm::mat4(1.0f) });

	for (auto& vertex : vertices) {
		vertex.Position += pos;
	}

	return std::make_shared<MeshSource>(vertices, indices, submeshes);
}

std::shared_ptr<Nut::MeshSource> Nut::GeometryGenerator::CreateCircle(const glm::vec3& pos /*= glm::vec3(0.f)*/, float radius /*= 1.0f*/, int sliceCount /*= 128*/)
{
	std::vector<Vertex> vertices;
	std::vector<SubMesh> submeshes;
	std::vector<Index> indices;

	// Generate vertices
	for (int i = 0; i < sliceCount; ++i) {
		float theta = 2.0f * glm::pi<float>() * i / sliceCount;

		glm::vec3 position(
			radius * cosf(theta),
			radius * sinf(theta),
			0.0f
		);

		glm::vec3 normal(0.f, 1.f, 0.f);
		glm::vec2 texCoord(static_cast<float>(i) / sliceCount, 0.5f);

		vertices.push_back(Vertex(position, normal, texCoord, glm::vec3(1.f), glm::vec3(0.f)));
	}
	vertices.insert(vertices.begin(), Vertex(pos, glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f), glm::vec3(1.f), glm::vec3(0.f)));

	// Generate indices
	for (int i = 1; i <= sliceCount; ++i) {
		int first = i;
		int second = first + 1;

		if (i == sliceCount) {
			second = 1;
		}

		indices.push_back(Index(second, 0, first));
	}

	submeshes.push_back(SubMesh{ 0, 0, 0, static_cast<uint32_t>(vertices.size()), static_cast<uint32_t>(indices.size() * 3), glm::mat4(1.0f), glm::mat4(1.0f) });

	for (auto& vertex : vertices) {
		vertex.Position += pos;
	}

	std::shared_ptr<MeshSource> meshSource = std::make_shared<MeshSource>(vertices, indices, submeshes);
	meshSource->m_Materials.push_back(Material::Create(Shader::Create("EdgeHighlightShader")));

	return meshSource;
}
