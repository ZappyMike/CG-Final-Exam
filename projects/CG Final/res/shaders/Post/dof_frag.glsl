#version 420

layout(location = 0) in vec2 inUV;

uniform float BrightnessThreshold;
uniform float u_DepthLimit;
uniform vec3 u_camPos;
//uniform mat4 u_LightSpaceMatrix;

uniform vec4 u_lightPos;
uniform vec4 u_lightColor;

layout (binding = 0) uniform sampler2D s_albedoTex;
layout (binding = 1) uniform sampler2D s_normalsTex;
layout (binding = 2) uniform sampler2D s_specularTex;
layout (binding = 3) uniform sampler2D s_positionTex;

layout(binding = 4) uniform sampler2D u_Depth;

float s1 = 20.0f;
float f = 0.0303f;
float A = 0.6062f;
float maxCoC = 1.0f;

	//Albedo
	vec4 textureColor = texture(s_albedoTex, inUV);
	//Normals
	vec3 inNormal = (normalize(texture(s_normalsTex, inUV).rgb) * 2.0) - 1.0;
	//Specular
	float texSpec = texture(s_specularTex, inUV).r;
	//Positions
	vec3 fragPos = texture(s_positionTex, inUV).rgb;

	vec4 depthColor = texture(u_Depth, inUV);

out vec4 FragColor;

float circleOfConfusion()
{
	vec4 fragPosLightSpace = depthColor * vec4(fragPos, 1.0);
	vec3 projectionCoordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;

	float depth = projectionCoordinates.z;

	float s2 = abs(depth);

	float CoC = A * (abs(s2 - s1) / s2 * (f / (s1 - f)));

	float sensorHeight = 0.024f;

	float percentOfSensor = CoC / sensorHeight;

	float BlurFactor = clamp(percentOfSensor, 0.0f, maxCoC);

	return BlurFactor;
}

vec3 pixelColor()
{
	vec3 lightPosition = normalize(u_lightPos.xyz - u_camPos);
	vec3 N = normalize(inNormal);

	float diffuse = max(0.0, dot(N, lightPosition));

	if(diffuse <= 0.0)
		diffuse = 1.0;
	else if(diffuse <= 0.75)
		diffuse = 0.25;
	else if(diffuse <= 0.5)
		diffuse = 0.5;
	else if(diffuse <= 0.25)
		diffuse = 0.0;
	else 
		diffuse = 0.0;

	return vec3(0.5, 0.5, 0.5) * (diffuse * u_DepthLimit) + u_lightColor.rgb;
}

void main() 
{
	FragColor.rgb = pixelColor();
	FragColor.a = circleOfConfusion();
}