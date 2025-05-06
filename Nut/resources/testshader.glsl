# type Vertex
#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) uniform float aTime;
layout(location = 3) uniform float aDissolveCoefficient;
layout(location = 4) out vec3 ClipLine;
layout(location = 5) out vec3 vPos;

void main()
{
	vec2 N = normalize(vec2(-1.0, 1.0));
	float Clipdistance = cos(aTime) * aDissolveCoefficient;
	ClipLine = vec3(N, Clipdistance);
	vPos = aPos;
	
	gl_Position = vec4(aPos, 1.0);
}

# type Fragment
#version 460 core

const vec4 color1 = vec4(0.38, 0.12, 0.93, 1.0);
const vec4 color2 = vec4(1.00, 0.90, 0.30, 1.0);
const vec4 color3 = vec4(1.00, 0.30, 0.30, 1.0);

layout(location = 4) in vec3 ClipLine;
layout(location = 5) in vec3 vPos;

float noise(vec2 st);
layout(location = 0) uniform float aNoiseCoefficient;

layout(location = 0) out vec4 FragColor;

void main()
{
	float noiseValue = noise(vPos.xy * aNoiseCoefficient);
	float dist = dot(vPos.xy, ClipLine.xy) + ClipLine.z - noiseValue;
	float alpha = step(0.0, dist);
	vec4 color23 = mix(color2, color3, (1.0 - step(0.2, dist)) * dist * 5.0);
	vec4 res = mix(color23, color1, step(0.2, dist));
	res.a = alpha;
	FragColor = res;
}

vec2 random2(vec2 st) {
  float d1 = dot(st, vec2(12.3, 32.1));
  float d2 = dot(st, vec2(45.6, 65.4));
  
  st = vec2(d1, d2);
  return fract(sin(st) * 78.9) * 2.0 - 1.0;
}

float noise(vec2 st) {
  vec2 i = floor(st);
  vec2 f = fract(st);

  vec2 u = smoothstep(0.0, 1.0, f);
  
  float d1 = dot(random2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0));
  float d2 = dot(random2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0));
  float d3 = dot(random2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0));
  float d4 = dot(random2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0));
  return mix(mix(d1, d2, u.x), mix(d3, d4, u.x), u.y);
}