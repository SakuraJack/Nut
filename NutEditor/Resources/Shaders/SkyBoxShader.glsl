# type Vertex
#version 460 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
	mat4 ViewProjectionMatrix;
	mat4 InverseViewProjectionMatrix;
	mat4 ProjectionMatrix;
	mat4 InverseProjectionMatrix;
	mat4 ViewMatrix;
	mat4 InverseViewMatrix;
	vec2 NDCToViewMul;
	vec2 NDCToViewAdd;
	vec2 DepthUnpackConsts;
	vec2 CameraTanHalfFOV;
} u_Camera;

layout(location = 0) out vec3 v_Position;

void main()
{
	mat3 rotation = mat3(u_Camera.ViewMatrix);
	v_Position = a_Position;
	mat4 rotView = mat4(rotation); // 构造无平移的view矩阵
	gl_Position = u_Camera.ProjectionMatrix * rotView * vec4(a_Position, 1.0);
}

# type Fragment
#version 460 core

layout(location = 0) out vec4 finalColor;

layout(binding = 1) uniform samplerCube u_Texture;

layout(location = 0) in vec3 v_Position;

void main()
{
	finalColor = texture(u_Texture, normalize(v_Position));
	finalColor.a = 1.0;
}