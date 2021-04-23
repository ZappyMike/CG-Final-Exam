#version 420

layout(location = 0) in vec2 inUV;

layout (binding = 0) uniform sampler2D s_albedoTex;
layout (binding = 1) uniform sampler2D s_normalsTex;
layout (binding = 2) uniform sampler2D s_specularTex;
layout (binding = 3) uniform sampler2D s_positionTex;


out vec4 FragColor;

//layout (binding = 0) uniform sampler2D u_FinishedFrame;

void main() 
{
	vec4 color = texture(s_albedoTex, inUV);
	//Normals
	vec3 inNormal = (normalize(texture(s_normalsTex, inUV).rgb) * 2.0) - 1.0;
	//Specular
	float texSpec = texture(s_specularTex, inUV).r;
	//Positions
	vec3 fragPos = texture(s_positionTex, inUV).rgb;

	FragColor.rgb = color.rgb; 
	FragColor.a = color.a; 
}