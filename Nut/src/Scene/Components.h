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
		// ֻ��ͨ��SetRotation()��SetRotationEuler()��������ת
		// ŷ���ǻ�������������� ������תӦ�ô洢Ϊ��Ԫ�� ������Ԫ������������ֱ�� �˸�������ʹ��ŷ����
		// ����򵥴洢��Ԫ��Ȼ��ͨ��Euler->Quat->Euler��ת���ǲ����е� ��Ϊŷ��������Ԫ��֮���ת���Ƕ�ֵ��
		// ���Դ洢Euler��Ϊ�˲����Ķ��� ������Ԫ����Ϊʵ�ʼ�����ת�Ķ���
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

			// ����ŷ��������Ԫ��ת���Ķ������ŷ����"��ת"
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