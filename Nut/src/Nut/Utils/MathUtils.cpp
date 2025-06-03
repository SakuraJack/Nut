#include "ntpch.h"
#include "MathUtils.h"

bool Nut::Math::DecomposeMatrix(const glm::mat4& matrix, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale)
{
	glm::mat4 LocalMatrix = matrix;

	if (glm::epsilonEqual(LocalMatrix[3][3], 0.0f, glm::epsilon<float>()))
		return false;

	// 获取平移
	translation = glm::vec3(LocalMatrix[3]);
	LocalMatrix[3] = glm::vec4(0, 0, 0, LocalMatrix[3].w);

	// 获取缩放 template<length_t C, length_t R, typename T, qualifier Q = defaultp> struct mat;
	glm::vec3 Row[3];
	for (glm::length_t i = 0; i < 3; ++i)
		for (glm::length_t j = 0; j < 3; ++j)
			Row[i][j] = LocalMatrix[i][j];

	scale.x = glm::length(Row[0]);
	Row[0] = Row[0] / scale.x;

	scale.y = glm::length(Row[1]);
	Row[1] = Row[1] / scale.y;

	scale.z = glm::length(Row[2]);
	Row[2] = Row[2] / scale.z;

	// 获取旋转 四元数
	int i, j, k = 0;
	float root, trace = Row[0].x + Row[1].y + Row[2].z;
	if (trace > 0.0f) {
		root = glm::sqrt(trace + 1.0f);
		rotation.w = root * 0.5f;
		root = 0.5f / root;
		rotation.x = root * (Row[1].z - Row[2].y);
		rotation.y = root * (Row[2].x - Row[0].z);
		rotation.z = root * (Row[0].y - Row[1].x);
	}
	else {
		static int Next[3] = { 1, 2, 0 };
		i = 0;
		if (Row[1].y > Row[0].x)
			i = 1;
		if (Row[2].z > Row[i][i])
			i = 2;
		j = Next[i];
		k = Next[j];

		root = glm::sqrt(Row[i][i] - Row[j][j] - Row[k][k] + 1.0f);

		rotation[i] = root * 0.5f;
		root = 0.5f / root;
		rotation.w = root * (Row[j][k] - Row[k][j]);
		rotation[j] = root * (Row[i][j] + Row[j][i]);
		rotation[k] = root * (Row[i][k] + Row[k][i]);
	}

	return true;
}
