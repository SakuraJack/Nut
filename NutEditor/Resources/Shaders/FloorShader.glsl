# type Vertex
#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;
// Transform buffer
layout(location = 5) in vec4 a_MRow0;
layout(location = 6) in vec4 a_MRow1;
layout(location = 7) in vec4 a_MRow2;

layout(location = 0) out vec2 v_TexCoord;

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

void main()
{
	mat4 transform = mat4(
		vec4(a_MRow0.x, a_MRow1.x, a_MRow2.x, 0.0),
		vec4(a_MRow0.y, a_MRow1.y, a_MRow2.y, 0.0),
		vec4(a_MRow0.z, a_MRow1.z, a_MRow2.z, 0.0),
		vec4(a_MRow0.w, a_MRow1.w, a_MRow2.w, 1.0)
	);
	vec4 pos = u_Camera.ViewProjectionMatrix * transform * vec4(a_Position, 1.0);
	gl_Position = pos;
	v_TexCoord = a_TexCoord.xy;
}

# type Fragment
#version 460 core

layout(location = 0) out vec4 finalColor;

layout(location = 0) in vec2 v_TexCoord;

void main()
{
	float border = 0.05;
	if (v_TexCoord.x < border || v_TexCoord.x > 1.0 - border || v_TexCoord.y < border || v_TexCoord.y > 1.0 - border)
		finalColor = vec4(0.0, 0.0, 0.0, 1.0);
	else
		finalColor = vec4(0.0, 0.0, 1.0, 1.0);
}