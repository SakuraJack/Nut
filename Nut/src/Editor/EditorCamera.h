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

		glm::vec3 GetUpDirection() const; // 获取上方向
		glm::vec3 GetRightDirection() const; // 获取右方向
		glm::vec3 GetForwardDirection() const; // 获取前方向
		glm::quat GetOrientation() const; // 获取四元数方向

		CameraMode GetCameraMode() const { return m_CameraMode; } // 获取摄像机模式
		const glm::vec3& GetPosition() const { return m_Position; } // 获取位置
		const float GetDistance() const { return m_Distance; } // 获取距离
		inline void SetDistance(float distance) { m_Distance = distance; } // 设置距离

		float GetFOV() const { return m_FOV; } // 获取视场角
		float GetAspectRatio() const { return m_AspectRatio; } // 获取宽高比
		float GetNearPlane() const { return m_NearPlane; } // 获取近裁剪面
		float GetFarPlane() const { return m_FarPlane; } // 获取远裁剪面
		float GetPitch() const { return m_Pitch; } // 获取俯仰角
		float GetYaw() const { return m_Yaw; } // 获取偏航角
		float GetCameraSpeed() const;

		static std::shared_ptr<EditorCamera> Create(const float degreeFOV, const float width, const float height, const float nearP, const float farP);
	private:
		glm::vec3 CalculatePosition() const; // 计算位置
		void UpdateViewMatrix();

		bool OnMouseScroll(MouseScrolledEvent& e); // 滚轮缩放
		void MouseRotate(const glm::vec2& delta); // 鼠标旋转
		void MousePan(const glm::vec2& delta); // 鼠标平移
		void MouseZoom(float delta); // 鼠标缩放

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;

	private:
		glm::mat4 m_ViewMatrix, m_LastViewMatrix;
		glm::vec3 m_Position, m_Direction, m_FocusPoint{};

		glm::vec2 m_InitialMousePosition;
		glm::vec3 m_InitialFocusPoint, m_InitialRotation;

		// 透视投影参数
		float m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane;

		float m_Distance;
		float m_Pitch, m_Yaw;	// 俯仰角、偏航角

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