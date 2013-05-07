#include "static_bone_shader.h"

static const char* pEffectFile = SHADER_PATH "null/null.glsl";

StaticNullShader::StaticNullShader() : Shader(pEffectFile)
{   
}

bool StaticNullShader::Init()
{
    if (!CompileProgram("StaticNullShader")) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}


void StaticNullShader::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

