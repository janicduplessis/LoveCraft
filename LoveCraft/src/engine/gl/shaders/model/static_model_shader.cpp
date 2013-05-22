#include "static_model_shader.h"
#include "util/tool.h"
#include <string>
#include <cassert>
#include "glfx.h"

static const char* shaderFile = SHADER_PATH "model/model.glsl";

StaticModelShader::StaticModelShader() : Shader(shaderFile)
{

}

StaticModelShader::~StaticModelShader()
{

}

bool StaticModelShader::Init()
{
	if (!CompileProgram("StaticModelGeometryPass"))
		return false;

	//glfxGenerateSampler(m_effect, "Sampler");
	m_WVPLocation = GetUniformLocation("gWVP");
	m_worldLocation = GetUniformLocation("gWorld");
	m_colorSamplerLocation = GetUniformLocation("gColorTexture");
	m_normalSamplerLocation = GetUniformLocation("gNormalTexture");

	// Valide les uniforms
	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_worldLocation == INVALID_UNIFORM_LOCATION)
			return false;

	return true;
}

void StaticModelShader::SetWVP( const Matrix4f& WVP )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.GetInternalValues());
}

void StaticModelShader::SetWorld( const Matrix4f& world )
{
	glUniformMatrix4fv(m_worldLocation, 1, GL_TRUE, (const GLfloat*)world.GetInternalValues());
}

void StaticModelShader::SetColorTextureUnit( unsigned int unit )
{
	glUniform1i(m_colorSamplerLocation, unit);
}

void StaticModelShader::SetNormalTextureUnit( unsigned int unit )
{
	glUniform1i(m_normalSamplerLocation, unit);
}
