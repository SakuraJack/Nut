#pragma once

#include "Mesh.h"

namespace Nut {

	class MeshFactory
	{
	public:
		static AssetHandle CreateBoxTexture(const glm::vec3& size);
		static AssetHandle CreateBoxGeometry(const glm::vec3& size);
		static AssetHandle CreateSphere(float radius);
		static AssetHandle CreateCapsule(float radius, float height);
	};
}