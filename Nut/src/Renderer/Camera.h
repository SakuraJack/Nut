#pragma once
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace Nut {

	enum class CameraMode
	{
		NONE, FLYCAM, ARCBALL, FPSCAM //���������
	};

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);
		Camera(const float fov, const float width, const float height, const float nearP, const float farP);
		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }	//  ��ȡͶӰ����

		void SetProjectionMatrix(const glm::mat4& projection);	//  ����ͶӰ����
		void SetPerspectiveProjectionMatrix(const float fov, const float width, const float height, const float nearP, const float farP);	//  ����͸��ͶӰ����
		void SetOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float nearP, const float farP);	//  ��������ͶӰ����

	private:
		glm::mat4 m_ProjectionMatrix;	//  ͶӰ����
	};
}