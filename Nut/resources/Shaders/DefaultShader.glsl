# type Vertex
#version 460 core

layout(location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform u_MatrixBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

# type Fragment
#version 460 core

layout(location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
