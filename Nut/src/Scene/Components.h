#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/UUID.h"
#include "Renderer/Mesh.h"
#include "Utils/MathUtils.h"

namespace Nut {

	struct IDComponent
	{
		UUID ID;
	};

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };

	private:
		// 只能通过SetRotation()和SetRotationEuler()来设置旋转
		// 欧拉角会产生万向锁问题 所以旋转应该存储为四元数 但是四元数不符合人类直觉 人更倾向于使用欧拉角
		// 如果简单存储四元数然后通过Euler->Quat->Euler来转换是不可行的 因为欧拉角与四元数之间的转换是多值的
		// 所以存储Euler作为人操作的对象 但是四元数作为实际计算旋转的对象
		glm::vec3 RotationEuler{ 0.0f, 0.0f, 0.0f };
		glm::quat RotationQuat{ 1.0f, 0.0f, 0.0f, 0.0f };

	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), Translation) *
				glm::toMat4(RotationQuat) *
				glm::scale(glm::mat4(1.0f), Scale);
		}

		void SetTransform(const glm::mat4& transform) {
			Math::DecomposeMatrix(transform, Translation, RotationQuat, Scale);
			RotationEuler = glm::eulerAngles(RotationQuat);
		}

		glm::vec3 GetRoationEuler() const {
			return RotationEuler;
		}

		void SetRotationEuler(const glm::vec3& euler) {
			RotationEuler = euler;
			RotationQuat = glm::quat(glm::radians(euler));
		}

		glm::quat GetRotationQuat() const {
			return RotationQuat;
		}

		void SetRotationQuat(const glm::quat& quat) {
			auto originEuler = RotationEuler;
			RotationQuat = quat;
			RotationEuler = glm::eulerAngles(RotationQuat);

			// 避免欧拉角与四元数转换的多解引发欧拉角"翻转"
			if (std::fabs(RotationEuler.x - originEuler.x) == glm::pi<float>() &&
				std::fabs(RotationEuler.z - originEuler.z) == glm::pi<float>())
			{
				RotationEuler.x = originEuler.x;
				RotationEuler.y	= glm::pi<float>() - RotationEuler.y;
				RotationEuler.z = originEuler.z;
			}
		}
	};

	struct MeshComponent
	{
		AssetHandle Mesh;
		uint32_t SubmeshIndex = 0;
		bool Visiable = true;
		
		MeshComponent() = default;
		MeshComponent(const MeshComponent& other)
			: Mesh(other.Mesh), SubmeshIndex(other.SubmeshIndex) {}
		MeshComponent(AssetHandle mesh, uint32_t submeshIndex = 0)
			: Mesh(mesh), SubmeshIndex(submeshIndex) {}
	};

	struct StaticMeshComponent
	{
		AssetHandle StaticMesh;
		bool Visiable = true;

		StaticMeshComponent() = default;
		StaticMeshComponent(const StaticMeshComponent& other)
			: StaticMesh(other.StaticMesh) {}
		StaticMeshComponent(AssetHandle staticMesh)
			: StaticMesh(staticMesh) {}
	};
}