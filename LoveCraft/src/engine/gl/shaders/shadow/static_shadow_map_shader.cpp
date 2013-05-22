#include <glfx.h>

#include "static_shadow_map_shader.h"

static const char* shaderFile = SHADER_PATH "shadow/shadow_map.glsl";

StaticShadowMapShader::StaticShadowMapShader() : Shader(shaderFile)
{

}

bool StaticShadowMapShader::Init()
{
	if (!CompileProgram("StaticShadowMap")) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_textureLocation = GetUniformLocation("gShadowMap");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_textureLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	return true;
}

void StaticShadowMapShader::SetWVP( const Matrix4f& wvp )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)wvp.m);
}

void StaticShadowMapShader::SetTextureUnit( unsigned int textureUnit )
{
	glUniform1i(m_textureLocation, textureUnit);
}

