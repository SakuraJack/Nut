#include "ntpch.h"
#include "Camera.h"

Nut::Camera::Camera(const glm::mat4& projection)
	: m_ProjectionMatrix(projection)
{

}

Nut::Camera::Camera(const float degreeFov, const float width, const float height, const float nearP, const float farP)
	: m_ProjectionMatrix(glm::perspectiveFov(glm::radians(degreeFov), width, height, nearP, farP))
{

}

void Nut::Camera::SetProjectionMatrix(const glm::mat4& projection)
{
	m_ProjectionMatrix = projection;
}

void Nut::Camera::SetPerspectiveProjectionMatrix(const float fov, const float width, const float height, const float nearP, const float farP)
{
	m_ProjectionMatrix = glm::perspectiveFov(fov, width, height, nearP, farP);
}

void Nut::Camera::SetOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float nearP, const float farP)
{
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top, nearP, farP);
}

