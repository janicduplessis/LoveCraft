#include "skyboxshader.h"
#include "util/tool.h"


SkyboxShader::~SkyboxShader()
{

}

SkyboxShader::SkyboxShader()
{

}

bool SkyboxShader::Init()
{
	if (!Shader::Init())
		return false;

	if (!AddShader(GL_VERTEX_SHADER, SHADER_PATH "skyboxshader.vert", true))
		return false;

	if (!AddShader(GL_FRAGMENT_SHADER, SHADER_PATH "skyboxshader.frag", true))
		return false;

	if (!Link())
		return false;

	m_WVPLocation = BindUniform("gWVP");
	m_textureLocation = BindUniform("gCubemapTexture");

	return true;
}

void SkyboxShader::SetWVP( const Matrix4f& wvp )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)wvp.GetInternalValues());
}

void SkyboxShader::SetTextureUnit( unsigned int textureUnit )
{
	glUniform1i(m_textureLocation, textureUnit);
}

