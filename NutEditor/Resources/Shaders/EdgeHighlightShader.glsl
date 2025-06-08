# type Vertex
#version 460 core

layout(location = 0) in vec3 aPos;

layout(std140, binding = 0) uniform u_MatrixBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

layout(location = 1) out vec3 vPos;
layout(location = 2) out mat4 InverseModel;

void main()
{
	vec4 pos = model * vec4(aPos, 1.0);
	vPos = pos.xyz;
	gl_Position = projection * view * pos;
	InverseModel = inverse(model);
}

# type Fragment
#version 460 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) in vec3 vPos;
layout(location = 2) in mat4 InverseModel;

layout(std140, binding = 1) uniform u_DetectParameter
{
	float radius;
	float edgeWidth;
	vec3 edgeColor;
	vec3 fillColor;
	float fillAlpha;
	vec2 mousePos;
	float highlightDist;
};

void main()
{
	vec4 localPos4 = InverseModel * vec4(vPos, 1.0);
	vec2 localPos = localPos4.xy;

	float dist = length(localPos);
	float halfEdge = edgeWidth * 0.5;
	if (dist > radius + halfEdge)
		discard;

	float mouseDist = length(mousePos);
	bool mouseNearEdge = abs(mouseDist - radius) <= (halfEdge + highlightDist);
	if (abs(dist - radius) <= halfEdge)
	{
		if (mouseNearEdge)
			FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		else 
			FragColor = vec4(edgeColor, 1.0);

	}
	else
	{
		FragColor = vec4(fillColor, 0.2);
	}
}