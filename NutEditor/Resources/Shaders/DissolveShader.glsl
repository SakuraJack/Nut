# type Vertex
#version 460 core

layout(location = 0) in vec3 aPos;

layout(location = 1) out vec4 ClipLine;
layout(location = 2) out vec4 vPos;

// Texture units
layout(location = 3) uniform sampler2D noiseTexture1;
layout(location = 4) uniform sampler2D noiseTexture2;
layout(location = 5) uniform sampler3D noiseTexture3;
layout(location = 6) uniform sampler3D noiseTexture4;
layout(location = 7) uniform samplerCube noiseTexture5;

layout(std140, binding = 0) uniform u_MatrixBuffer
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

layout(std140, binding = 1) uniform u_DissolveBuffer
{
	float u_DissolveEdgeWidth;
	float u_DissolvePercent;
	float u_NoiseDensity;
};

void main()
{
	vec3 N = normalize(vec3(0.0, 1.0, 0.0));
	vPos = model * vec4(aPos, 1.0);
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

# type Fragment
#version 460 core

const vec4 color1 = vec4(0.38, 0.12, 0.93, 1.0);
const vec4 color2 = vec4(1.00, 0.90, 0.30, 1.0);
const vec4 color3 = vec4(1.00, 0.30, 0.30, 1.0);

layout(location = 1) in vec4 ClipLine;
layout(location = 2) in vec4 vPos;

layout(location = 0) out vec4 FragColor;

layout(std140, binding = 1) uniform DissolveBuffer
{
	float u_DissolveEdgeWidth;
	float u_DissolvePercent;
	float u_NoiseDensity;
};

float noise(vec3 st);

void main()
{
	float noiseValue = noise(vPos.xyz * u_NoiseDensity);
	float dist = noiseValue + u_DissolvePercent;
	float alpha = step(0.0, dist);
	vec4 color23 = mix(color2, color3, (1.0 - step(u_DissolveEdgeWidth, dist)) * dist * (1.0 / u_DissolveEdgeWidth));
	vec4 res = mix(color23, color1, step(u_DissolveEdgeWidth, dist));
	res.a = alpha;
	FragColor = res;
	// FragColor = vec4(dist, dist, dist, 1.0);
}

vec3 random(vec3 st) {
	float a = fract(sin(dot(st, vec3(12.9898, 78.233, 45.164))) * 43758.5453123);
	float b = fract(sin(dot(st, vec3(93.9898, 67.345, 12.345))) * 43758.5453123);
	float c = fract(sin(dot(st, vec3(45.1234, 23.4567, 89.1234))) * 43758.5453123);
	return normalize(vec3(a, b, c));
}

float noise(vec3 st) {
  vec3 i = floor(st);
  vec3 f = fract(st);

  vec3 u = f * f * (3.0 - 2.0 * f);
  //vec3 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0); // ƽ����ֵ

  vec3 i000 = i; // ����½�
  vec3 i100 = i + vec3(1.0, 0.0, 0.0); // �Һ��½�
  vec3 i010 = i + vec3(0.0, 1.0, 0.0); // ����Ͻ�
  vec3 i110 = i + vec3(1.0, 1.0, 0.0); // �Һ��½�
  vec3 i001 = i + vec3(0.0, 0.0, 1.0); // ��ǰ�½�
  vec3 i101 = i + vec3(1.0, 0.0, 1.0); // ��ǰ�½�
  vec3 i011 = i + vec3(0.0, 1.0, 1.0); // ��ǰ�Ͻ�
  vec3 i111 = i + vec3(1.0, 1.0, 1.0); // ��ǰ�Ͻ�

  // ����ÿ�����������ݶ��������
  float f000 = dot(random(i000), u + i - i000); // ����½�
  float f100 = dot(random(i100), u + i - i100); // �Һ��½�
  float f010 = dot(random(i010), u + i - i010); // ����Ͻ�
  float f110 = dot(random(i110), u + i - i110); // �Һ��Ͻ�
  float f001 = dot(random(i001), u + i - i001); // ��ǰ�½�
  float f101 = dot(random(i101), u + i - i101); // ��ǰ�½�
  float f011 = dot(random(i011), u + i - i011); // ��ǰ�Ͻ�
  float f111 = dot(random(i111), u + i - i111); // ��ǰ�Ͻ�
  

  // ��x���ֵ
  float nx00 = mix(f000, f100, u.x);
  float nx10 = mix(f010, f110, u.x);
  float nx01 = mix(f001, f101, u.x);
  float nx11 = mix(f011, f111, u.x);

  // ��y���ֵ
  float nxy0 = mix(nx00, nx10, u.y);
  float nxy1 = mix(nx01, nx11, u.y);

  // ��z���ֵ
  float nxyz = mix(nxy0, nxy1, u.z);
  return nxyz;
}