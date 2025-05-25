#pragma once
#include "Renderer/Camera.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Core/Timestep.h"
#include <glm/detail/type_quat.hpp>

namespace Nut {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const float degreeFOV, const float width, const float height, const float nearP, const float farP);
		void Init();

		void Focus(const glm::vec3& focusPoint);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return GetProjectionMatrix() * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const; // ��ȡ�Ϸ���
		glm::vec3 GetRightDirection() const; // ��ȡ�ҷ���
		glm::vec3 GetForwardDirection() const; // ��ȡǰ����
		glm::quat GetOrientation() const; // ��ȡ��Ԫ������

		CameraMode GetCameraMode() const { return m_CameraMode; } // ��ȡ�����ģʽ
		const glm::vec3& GetPosition() const { return m_Position; } // ��ȡλ��
		const float GetDistance() const { return m_Distance; } // ��ȡ����
		inline void SetDistance(float distance) { m_Distance = distance; } // ���þ���

		float GetFOV() const { return m_FOV; } // ��ȡ�ӳ���
		float GetAspectRatio() const { return m_AspectRatio; } // ��ȡ��߱�
		float GetNearPlane() const { return m_NearPlane; } // ��ȡ���ü���
		float GetFarPlane() const { return m_FarPlane; } // ��ȡԶ�ü���
		float GetPitch() const { return m_Pitch; } // ��ȡ������
		float GetYaw() const { return m_Yaw; } // ��ȡƫ����
		float GetCameraSpeed() const;

		static std::shared_ptr<EditorCamera> Create(const float degreeFOV, const float width, const float height, const float nearP, const float farP);
	private:
		glm::vec3 CalculatePosition() const; // ����λ��
		void UpdateViewMatrix();

		bool OnMouseScroll(MouseScrolledEvent& e); // ��������
		void MouseRotate(const glm::vec2& delta); // �����ת
		void MousePan(const glm::vec2& delta); // ���ƽ��
		void MouseZoom(float delta); // �������

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		glm::mat4 m_ViewMatrix, m_LastViewMatrix;
		glm::vec3 m_Position, m_Direction, m_FocusPoint{};

		glm::vec2 m_InitialMousePosition;
		glm::vec3 m_InitialFocusPoint, m_InitialRotation;

		// ͸��ͶӰ����
		float m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane;

		float m_Distance;
		float m_Pitch, m_Yaw;	// �����ǡ�ƫ����

		float m_PitchDelta = 0.f, m_YawDelta = 0.f;
		glm::vec3 m_RotationDelta{};
		glm::vec3 m_PositionDelta{};
		glm::vec3 m_RightDirection{};

		float m_MinFocusDistance{ 100.0f };
		float m_NormalSpeed{ 0.002f };
		constexpr static float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };

		CameraMode m_CameraMode{ CameraMode::ARCBALL };

		uint32_t m_ViewportWidth{ 1280 }, m_ViewportHeight{ 720 };
	};
}