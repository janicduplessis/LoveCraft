#include "lightingshader.h"

LightingShader::LightingShader()
{

}

LightingShader::~LightingShader()
{

}

bool LightingShader::Init()
{
	Shader::Init();
	AddShader(GL_VERTEX_SHADER, SHADER_PATH "lightingshader.vert");
	AddShader(GL_FRAGMENT_SHADER, SHADER_PATH "lightingshader.frag");
	Link();

	m_WVPLocation = BindUniform("gWVP");
	m_samplerLocation = BindUniform("gSampler");
	m_dirLightColorLocation = BindUniform("gDirectionalLight.Color");
	m_dirLightAmbientIntensityLocation = BindUniform("gDirectionalLight.AmbientIntensity");

	return true;
}

void LightingShader::SetWVP( const Matrix4f& WVP )
{
	//glUniformMatrix4fv(
}

void LightingShader::SetTextureUnit( unsigned int TextureUnit )
{

}

void LightingShader::SetDirectionalLight( const DirectionalLight& Light )
{

}

