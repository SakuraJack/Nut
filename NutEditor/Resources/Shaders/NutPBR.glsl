# type Vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

// Transform buffer
layout(location = 5) in vec4 a_MRow0;
layout(location = 6) in vec4 a_MRow1;
layout(location = 7) in vec4 a_MRow2;

struct VertexOutput
{
	vec3 WorldPosition;
	vec3 Normal;
	vec2 TexCoord;
	mat3 WorldNormals;
	mat3 WorldTransform;
	vec3 Binormal;

	mat3 CameraView;

	vec3 ShadowMapCoords[4];
	vec3 ViewPosition;
};

layout(location = 0) out VertexOutput Output;

// 强制gl_Position在所有着色器编译单元保持一致
// 避免浮点数优化 编译器差异导致微小差异 防止出现Z-fighting 阴影边缘错位
invariant gl_Position;

void main()
{
	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);
	vec4 worldPosition = transform * vec4(a_Position, 1.0);

	Output.WorldPosition = worldPosition.xyz;
	Output.Normal = normalize(mat3(transform) * a_Normal);
	Output.TexCoord = vec2(a_TexCoord.x, 1.0 - a_TexCoord.y);
	Output.WorldNormals = mat3(transform) * mat3(a_Tangent, a_Binormal, aNormal);
	Output.WorldTransform = mat3(transform);
	Output.Binormal = a_Binormal;

	Output.CameraView = mat3(u_Camera.ViewMatrix);
}