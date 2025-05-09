#pragma once
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace Nut {

	enum class CameraMode
	{
		NONE, FLYCAM, ARCBALL, FPSCAM //摄像机类型
	};

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection);
		Camera(const float fov, const float width, const float height, const float nearP, const float farP);
		virtual ~Camera() = default;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }	//  获取投影矩阵

		void SetProjectionMatrix(const glm::mat4& projection);	//  设置投影矩阵
		void SetPerspectiveProjectionMatrix(const float fov, const float width, const float height, const float nearP, const float farP);	//  设置透视投影矩阵
		void SetOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float nearP, const float farP);	//  设置正交投影矩阵

	private:
		glm::mat4 m_ProjectionMatrix;	//  投影矩阵
	};
}