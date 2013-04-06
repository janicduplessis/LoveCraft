#include "staticmodelshader.h"
#include "util/tool.h"
#include <string>
#include <cassert>
#include "glfx.h"

static const char* pEffectFile = SHADER_PATH "model.glsl";

StaticModelShader::StaticModelShader() : ShaderNew(pEffectFile)
{

}

StaticModelShader::~StaticModelShader()
{

}

bool StaticModelShader::Init()
{
	if (!CompileProgram("StaticModelGeometryPass"))
		return false;

	glfxGenerateSampler(m_effect, "Sampler");
	m_WVPLocation = GetUniformLocation("gWVP");
	m_worldLocation = GetUniformLocation("gWorld");

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

