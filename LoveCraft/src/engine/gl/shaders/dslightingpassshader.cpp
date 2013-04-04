#include <limits.h>
#include <string.h>

#include "dslightingpassshader.h"
#include "util/tool.h"

static const char* pEffectFile = SHADER_PATH "lightingpass.glsl";

DSLightingPassShader::DSLightingPassShader() : ShaderNew(pEffectFile)
{   
}

bool DSLightingPassShader::Init()
{
    m_WVPLocation = GetUniformLocation("gWVP");
	m_posTextureUnitLocation = GetUniformLocation("gPositionMap");
	m_colorTextureUnitLocation = GetUniformLocation("gColorMap");
	m_normalTextureUnitLocation = GetUniformLocation("gNormalMap");
    m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
    m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
    m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
    m_screenSizeLocation = GetUniformLocation("gScreenSize");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
        m_posTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_colorTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
		m_normalTextureUnitLocation == INVALID_UNIFORM_LOCATION ||
        m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
        m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
        m_screenSizeLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }

	return true;
}


void DSLightingPassShader::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}


void DSLightingPassShader::SetPositionTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_posTextureUnitLocation, TextureUnit);
}


void DSLightingPassShader::SetColorTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_colorTextureUnitLocation, TextureUnit);
}


void DSLightingPassShader::SetNormalTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_normalTextureUnitLocation, TextureUnit);
}


void DSLightingPassShader::SetEyeWorldPos(const Vector3f& EyePos)
{
	glUniform3f(m_eyeWorldPosLocation, EyePos.x, EyePos.y, EyePos.z);
}


void DSLightingPassShader::SetScreenSize(unsigned int Width, unsigned int Height)
{
    glUniform2f(m_screenSizeLocation, (float)Width, (float)Height);
}