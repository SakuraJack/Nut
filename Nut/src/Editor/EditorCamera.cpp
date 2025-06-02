#include "ntpch.h"
#include "EditorCamera.h"
#include "Core/Core.h"
#include "Core/Input.h"
#include "Core/Log.h"

#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

Nut::EditorCamera::EditorCamera(const float degreeFOV, const float width, const float height, const float nearP, const float farP)
	: Camera(degreeFOV, width, height, nearP, farP), m_FocusPoint(0.0f), m_FOV(degreeFOV), m_AspectRatio(width / height), m_NearPlane(nearP), m_FarPlane(farP)
{
	Init();
}

void Nut::EditorCamera::Init()
{
	const glm::vec3 position = {0, 0, -5}; // ��ʼλ��
	m_Distance = glm::distance(position, m_FocusPoint); // ���뽹��ľ���

	m_Yaw = 0.f; // ƫ���� ��Y����ת �������¿� ˳ʱ��+ ��ʱ��-
	m_Pitch = 0.f; // ������ ��X����ת ̧ͷ+ ��ͷ-

	m_Position = CalculatePosition(); // ����λ��
	const glm::quat orientation = GetOrientation(); // ��ȡ��Ԫ������
	m_Direction = glm::eulerAngles(orientation) * (180.f / glm::pi<float>()); // ���㷽��
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation); // ������ͼ����
	m_ViewMatrix = glm::inverse(m_ViewMatrix); // ��ת��ͼ����
}

void Nut::EditorCamera::Focus(const glm::vec3& focusPoint)
{
	m_FocusPoint = focusPoint; // ���ý���
	m_CameraMode = CameraMode::FLYCAM; 
	if (m_Distance > m_MinFocusDistance) {
		m_Distance -= m_Distance - m_MinFocusDistance;
		m_Position = m_FocusPoint - GetForwardDirection() * m_Distance; // ����λ��
	}
	m_Position = m_FocusPoint - GetForwardDirection() * m_Distance; // ����λ��
	UpdateViewMatrix(); // ������ͼ����
}

void Nut::EditorCamera::OnUpdate(Timestep ts)
{
	const glm::vec2& mouse { Input::GetMouseX(), Input::GetMouseY() }; // ��ȡ���λ��
	const glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.002f; // ��������ƶ�����

	if (Input::IsMouseButtonHeld(MouseButton::RightButton) && !Input::IsKeyHeld(KeyCode::LeftAlt)) {
		m_CameraMode = CameraMode::FLYCAM; // ���������ģʽ
		// TODO: �������
		// DisableMouse();
		const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
		const float speed = GetCameraSpeed();

		if (Input::IsKeyHeld(KeyCode::Q))
			m_PositionDelta -= ts.GetMilliseconds() * speed * glm::vec3{ 0.f, yawSign, 0.f };
		if (Input::IsKeyHeld(KeyCode::E))
			m_PositionDelta += ts.GetMilliseconds() * speed * glm::vec3{ 0.f, yawSign, 0.f };
		if (Input::IsKeyHeld(KeyCode::W))
			m_PositionDelta += ts.GetMilliseconds() * speed * GetForwardDirection();
		if (Input::IsKeyHeld(KeyCode::S))
			m_PositionDelta -= ts.GetMilliseconds() * speed * GetForwardDirection();
		if (Input::IsKeyHeld(KeyCode::A))
			m_PositionDelta -= ts.GetMilliseconds() * speed * GetRightDirection();
		if (Input::IsKeyHeld(KeyCode::D))
			m_PositionDelta += ts.GetMilliseconds() * speed * GetRightDirection();

		const float maxRate{ 0.12f };
		m_YawDelta += glm::clamp(yawSign * delta.x * RotationSpeed(), -maxRate, maxRate);
		m_PitchDelta += glm::clamp(delta.y * RotationSpeed(), -maxRate, maxRate);

		m_RightDirection = glm::cross(m_Direction, glm::vec3{ 0.f, yawSign, 0.f });

		m_Direction = glm::rotate(glm::normalize(glm::cross(glm::angleAxis(-m_PitchDelta, m_RightDirection),
			glm::angleAxis(-m_YawDelta, glm::vec3{ 0.f, yawSign, 0.f }))), m_Direction);

		const float distance = glm::distance(m_FocusPoint, m_Position);
		m_FocusPoint = m_Position + GetForwardDirection() * distance;
		m_Distance = distance;
	}
	else if (Input::IsKeyHeld(KeyCode::LeftAlt)) {
		m_CameraMode = CameraMode::ARCBALL;

		if (Input::IsMouseButtonHeld(MouseButton::MiddleButton)) {
			MousePan(delta);
		}
		else if (Input::IsMouseButtonHeld(MouseButton::LeftButton)) {
			MouseRotate(delta);
		}
		else if (Input::IsMouseButtonHeld(MouseButton::RightButton)) {
			MouseZoom((delta.x + delta.y) * 0.1f);
		}
	}

	m_InitialMousePosition = mouse;
	m_Position += m_PositionDelta;
	m_Yaw += m_YawDelta;
	m_Pitch += m_PitchDelta;

	if (m_CameraMode == CameraMode::ARCBALL)
		m_Position = CalculatePosition();

	UpdateViewMatrix();
}

void Nut::EditorCamera::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(NUT_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	dispatcher.Dispatch<WindowResizeEvent>(NUT_BIND_EVENT_FN(EditorCamera::OnWindowResized));
}

glm::vec3 Nut::EditorCamera::GetUpDirection() const
{
	return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Nut::EditorCamera::GetRightDirection() const
{
	return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
}

glm::vec3 Nut::EditorCamera::GetForwardDirection() const
{
	return glm::rotate(GetOrientation(), glm::vec3(0.f, 0.f, -1.f));
}

glm::quat Nut::EditorCamera::GetOrientation() const
{
	return glm::quat(glm::vec3(-m_Pitch - m_PitchDelta, -m_Yaw - m_YawDelta, 0.0f)); // ������Ԫ������;
	// glm�е���ת˳��Ϊ Z->X->Y 
}

float Nut::EditorCamera::GetCameraSpeed() const
{
	float speed = m_NormalSpeed;
	if (Input::IsKeyHeld(KeyCode::LeftControl))
		speed /= 2 - glm::log(m_NormalSpeed);
	if (Input::IsKeyHeld(KeyCode::LeftShift))
		speed *= 2 - glm::log(m_NormalSpeed);

	return glm::clamp(speed, MIN_SPEED, MAX_SPEED); // �����ٶȷ�Χ
}

std::shared_ptr<Nut::EditorCamera> Nut::EditorCamera::Create(const float degreeFOV, const float width, const float height, const float nearP, const float farP)
{
	return std::make_shared<EditorCamera>(degreeFOV, width, height, nearP, farP);
}

bool Nut::EditorCamera::OnWindowResized(WindowResizeEvent& e)
{
	if (e.GetHeight() == 0 || e.GetWidth() == 0) {
		m_ViewportWidth = 1;
		m_ViewportHeight = 1;
		return false; // ������С��
	}
	UpdateViewMatrix();
	m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
	SetPerspectiveProjectionMatrix(m_FOV, e.GetWidth(), e.GetHeight(), m_NearPlane, m_FarPlane);
	NUT_TRACE("EditorCamera resized to {0}x{1}", e.GetWidth(), e.GetHeight());
	return true;
}

glm::vec3 Nut::EditorCamera::CalculatePosition() const
{
	return m_FocusPoint - GetForwardDirection() * m_Distance + m_PositionDelta;
}

void Nut::EditorCamera::UpdateViewMatrix()
{
	const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f; // ��ȡƫ������
	const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection()); // ����ǰ������Ϸ���ļн�
	if (cosAngle * yawSign > 0.99f)
		m_PitchDelta = 0.0f; // ����нǽӽ�180�ȣ�����Ҫ��ת

	const glm::vec3 lookAt = m_Position + GetForwardDirection();
	m_Direction = glm::normalize(lookAt - m_Position);
	m_Distance = glm::distance(m_Position, m_FocusPoint);
	m_ViewMatrix = glm::lookAt(m_Position, lookAt, glm::vec3(0.f, yawSign, 0.f));

	m_YawDelta *= 0.6f;// ��ת˥��
	m_PitchDelta *= 0.6f;
	m_PositionDelta *= 0.8f; // ƽ��˥��
}

bool Nut::EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
{
	if (Input::IsMouseButtonHeld(MouseButton::RightButton)) {
		m_NormalSpeed += e.GetYOffset() * 0.3f * m_NormalSpeed;
		m_NormalSpeed = std::clamp(m_NormalSpeed, MIN_SPEED, MAX_SPEED);
	}
	else {
		MouseZoom(e.GetYOffset() * 0.1f);
		UpdateViewMatrix();
	}
	return true;
}

void Nut::EditorCamera::MouseRotate(const glm::vec2& delta)
{
	const float yawSign = GetUpDirection().y < 0.0f ? -1.0f : 1.0f;
	m_YawDelta += yawSign * delta.x * RotationSpeed();
	m_PitchDelta += delta.y * RotationSpeed();
}

void Nut::EditorCamera::MousePan(const glm::vec2& delta)
{
	auto [xSpeed, ySpeed] = PanSpeed();
	m_FocusPoint -= GetRightDirection() * delta.x * xSpeed * m_Distance;
	m_FocusPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
}

void Nut::EditorCamera::MouseZoom(float delta)
{
	m_Distance -= delta * ZoomSpeed();
	const glm::vec3 forward = GetForwardDirection();
	m_Position = m_FocusPoint - forward * m_Distance;
	if (m_Distance < 1.f) {
		m_FocusPoint += forward * m_Distance;
		m_Distance = 1.f;
	}
	m_PositionDelta += delta * ZoomSpeed() * forward;
}

std::pair<float, float> Nut::EditorCamera::PanSpeed() const
{
	const float x = glm::min(float(m_ViewportWidth / 1000.f), 2.4f);
	const float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
	const float y = glm::min(float(m_ViewportHeight / 1000.f), 2.4f);
	const float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

	return { xFactor, yFactor };
}

float Nut::EditorCamera::RotationSpeed() const
{
	return .3f;
}

float Nut::EditorCamera::ZoomSpeed() const
{
	float distance = m_Distance * 0.2f;
	distance = glm::max(distance, 0.0f);
	float speed = distance * distance;
	speed = glm::min(speed, 50.f);
	return speed;
}
