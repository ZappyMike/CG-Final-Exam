#include "DepthOfField.h"

void DepthOfField::Init(unsigned width, unsigned height)
{
	//initialize buffers
	int index = int(_buffers.size());
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	//_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	_buffers[index]->Init(width, height);
	index++;
	_buffers.push_back(new Framebuffer());
	_buffers[index]->AddColorTarget(GL_RGBA8);
	//_buffers[index]->AddDepthTarget();
	_buffers[index]->Init(width, height);

	//shaders
	index = int(_shaders.size());
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/cg_passthrough_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/dof_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/blur_Hori_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/blur_Vert_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();
	index++;
	_shaders.push_back(Shader::Create());
	_shaders[index]->LoadShaderPartFromFile("shaders/passthrough_vert.glsl", GL_VERTEX_SHADER);
	_shaders[index]->LoadShaderPartFromFile("shaders/Post/cg_composite_frag.glsl", GL_FRAGMENT_SHADER);
	_shaders[index]->Link();

	//pixel size
	_pixelSize = glm::vec2(1.0 / width, 1.0 / height);

	PostEffect::Init(width, height);
}

void DepthOfField::ApplyEffect(GBuffer* gBuffer, PostEffect* buffer)
{
	//passthrough
	_shaders[0]->Bind();

	//gBuffer->BindLighting();
	
	buffer->BindColorAsTexture(0, 0, 0);

	_buffers[0]->RenderToFSQ();

	//gBuffer->UnbindLighting();

	buffer->UnbindTexture(0);

	_shaders[0]->UnBind();

	//depth of field shader
	BindShader(1);
	
	gBuffer->BindLighting();

	_shaders[1]->SetUniform("u_lightPos", _lightPos);
	_shaders[1]->SetUniform("u_lightCol", _lightCol);
	_shaders[1]->SetUniform("u_camPos", _camPos);
	//_shaders[1]->SetUniformMatrix("u_LightSpaceMatrix", _lightSpaceViewProj);
	_shaders[1]->SetUniform("u_DepthLimit", _depthLimit);

	BindColorAsTexture(1, 0, 0);
	buffer->BindDepthAsTexture(0, 4);

	_buffers[1]->RenderToFSQ();

	UnbindTexture(0);

	gBuffer->UnbindLighting();

	UnbindShader();

	//blur
	for (int i = 0; i < 10; i++)
	{
		//horizontal
		BindShader(2);
		_shaders[2]->SetUniform("pixelSize", _pixelSize.x);

		BindColorAsTexture(1, 0, 0);

		_buffers[2]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();

		//vertical
		BindShader(3);
		_shaders[3]->SetUniform("pixelSize", _pixelSize.y);

		BindColorAsTexture(2, 0, 0);

		_buffers[1]->RenderToFSQ();

		UnbindTexture(0);

		UnbindShader();
	}

	//composition
	BindShader(4);

	_buffers[0]->BindColorAsTexture(0, 0);
	_buffers[1]->BindColorAsTexture(0, 1);

	_buffers[0]->RenderToFSQ();

	UnbindShader();
}

glm::vec4 DepthOfField::getLightPos()
{
	return _lightPos;
}

glm::vec4 DepthOfField::getLightCol()
{
	return _lightCol;
}

glm::vec3 DepthOfField::getCamPos()
{
	return _camPos;
}

float DepthOfField::getDepthLimit()
{
	return _depthLimit;
}

void DepthOfField::setLightPos(glm::vec4 lightPos)
{
	_lightPos = lightPos;
}

void DepthOfField::setLightCol(glm::vec4 lightCol)
{
	_lightCol = lightCol;
}

void DepthOfField::setCamPos(glm::vec3 camPos)
{
	_camPos = camPos;
}

void DepthOfField::setDepthLimit(float depthLimit)
{
	_depthLimit = depthLimit;
}

void DepthOfField::SetLightSpaceViewProj(glm::mat4 lightSpaceViewProj)
{
	_lightSpaceViewProj = lightSpaceViewProj;
}
