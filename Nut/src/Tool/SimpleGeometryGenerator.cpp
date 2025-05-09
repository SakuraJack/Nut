#include "ntpch.h"
#include "SimpleGeometryGenerator.h"

std::shared_ptr<Nut::Mesh> Nut::GeometryGenerator::CreateCube(const glm::vec3& pos /*= glm::vec3(0.f)*/, float sideLength /*= 1.0f*/)
{
	std::vector<glm::vec3> vertices = {
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f, -0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3( 0.5f, -0.5f,  0.5f),
		glm::vec3( 0.5f,  0.5f,  0.5f)
	};

	std::vector<glm::vec2> uvs = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f)
	};

	std::vector<SubMesh> submeshes = {
		SubMesh{0, 0, 8, 12, glm::mat4(1.0f), glm::mat4(1.0f)}
	};

	std::vector<GLuint> indices = {
		// Front face
		0, 2, 3,
		0, 3, 1,
		
		// Back face
		4, 5, 6,
		4, 6, 7,

		// Left face
		4, 7, 3,
		4, 3, 0,

		// Right face
		5, 6, 2,
		5, 2, 1,

		// Top face
		7, 6, 2,
		7, 2, 3,

		// Bottom face
		4, 5, 1,
		4, 1, 0
	};

	for (auto& vertex : vertices) {
		vertex *= sideLength;
		vertex += pos;
	}



	return std::make_shared<Mesh>(vertices, indices, submeshes);
}
