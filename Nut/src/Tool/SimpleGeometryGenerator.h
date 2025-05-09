#pragma once

#include <glm/glm.hpp>
#include "Renderer/Mesh.h"

namespace Nut {

	class GeometryGenerator
	{
	public:
		static std::shared_ptr<Mesh> CreateCube(const glm::vec3& pos = glm::vec3(0.f), float sideLength = 1.0f);
	};
}