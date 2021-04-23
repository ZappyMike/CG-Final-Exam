#version 420

layout(location = 0) in vec2 inUV;

out vec4 FragColor;
uniform float pixelSize;

layout(binding = 0) uniform sampler2D u_FinishedFrame;

void main() 
{
	FragColor = vec4(0.0, 0.0, 0.0, 0.0);

	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y - 4.0 * pixelSize)) * 0.12;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y - 3.0 * pixelSize)) * 0.06;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y - 2.0 * pixelSize)) * 0.09;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y - pixelSize)) * 0.16;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y)) * 0.15;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y +	pixelSize)) * 0.15;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y + 2.0 * pixelSize)) * 0.06;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y + 3.0 * pixelSize)) * 0.09;
	FragColor += texture(u_FinishedFrame, vec2(inUV.x, inUV.y + 4.0 * pixelSize)) * 0.12;
}