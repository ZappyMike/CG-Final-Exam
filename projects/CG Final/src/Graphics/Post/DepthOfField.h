#pragma once


#include "Graphics/Post/PostEffect.h"
#include "UniformBuffer.h"
#include "Graphics/GBuffer.h"
#include "Graphics/LUT.h"

class DepthOfField : public PostEffect
{
public:
	//Initializes framebuffer
	//Overrides post effect Init
	void Init(unsigned width, unsigned height) override;

	//Applies the effect to this buffer
	//passes the previous framebuffer with the texture to apply as parameter
	void ApplyEffect(PostEffect* buffer) override {};

	void ApplyEffect(GBuffer* gBuffer, PostEffect* buffer);

	//Getters
	glm::vec4 getLightPos();
	glm::vec4 getLightCol();
	glm::vec3 getCamPos();
	float getDepthLimit();

	//Setters
	void setLightPos(glm::vec4 lightPos);
	void setLightCol(glm::vec4 lightCol);
	void setCamPos(glm::vec3 camPos);
	void setDepthLimit(float depthLimit);

	void SetLightSpaceViewProj(glm::mat4 lightSpaceViewProj);

private:

	glm::mat4 _lightSpaceViewProj;
	glm::vec4 _lightPos;
	glm::vec4 _lightCol;
	glm::vec3 _camPos;

	glm::vec2 _pixelSize;
	float _depthLimit = 0.8f;
};
