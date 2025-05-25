# type Vertex
#version 460 core

layout(location = 0) in vec3 aPos;

layout(location = 1) out vec3 TexCoords;

layout(std140, binding = 0) uniform u_MatrixBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

void main()
{
	mat4 rotView = mat4(mat3(view));
	vec4 pos = projection * rotView * vec4(aPos, 1.0);
	gl_Position = pos.xyww;
	TexCoords = aPos;
}

# type Fragment
#version 460 core

layout(location = 1) in vec3 TexCoords;

layout(location = 0) out vec4 FragColor;

layout(location = 0) uniform samplerCube u_Skybox;
layout(location = 1) uniform vec4 u_Color;

void main()
{
	FragColor = texture(u_Skybox, TexCoords);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}