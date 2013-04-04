#include "nullshader.h"

static const char* pEffectFile = SHADER_PATH "null.glsl";

NullShader::NullShader() : ShaderNew(pEffectFile)
{   
}

bool NullShader::Init()
{
    if (!CompileProgram("NullShader")) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}


void NullShader::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

