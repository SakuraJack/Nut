#pragma once

#include <glm/glm.hpp>
#include "Renderer/Mesh.h"

namespace Nut {

	class GeometryGenerator
	{
	public:
		static std::shared_ptr<MeshSource> CreateCube(const glm::vec3& pos = glm::vec3(0.f), float sideLength = 1.0f);
		static std::shared_ptr<MeshSource> CreateSphere(const glm::vec3& pos = glm::vec3(0.f), float radius = 1.0f, int sliceCount = 20, int stackCount = 20);
		static std::shared_ptr<MeshSource> CreateCircle(const glm::vec3& pos = glm::vec3(0.f), float radius = 1.0f, int sliceCount = 128);
	};
}