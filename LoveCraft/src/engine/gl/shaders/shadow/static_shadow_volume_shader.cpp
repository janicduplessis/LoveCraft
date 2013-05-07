#include <glfx.h>

#include "static_shadow_volume_shader.h"

static const char* pEffectFile = SHADER_PATH "shadow/shadow_volume.glsl";

StaticShadowVolumeShader::StaticShadowVolumeShader() : Shader(pEffectFile)
{   
}

bool StaticShadowVolumeShader::Init()
{
    if (!CompileProgram("StaticShadowVolume")) {
        return false;
    }
    
    m_VPLocation = GetUniformLocation("gVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_lightPosLocation = GetUniformLocation("gLightPos");

    if (m_VPLocation == INVALID_UNIFORM_LOCATION ||
        m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
        m_lightPosLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }
            
    return true;
}

void StaticShadowVolumeShader::SetVP(const Matrix4f& VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)VP.m);    
}


void StaticShadowVolumeShader::SetWorldMatrix(const Matrix4f& WorldInverse)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
}

void StaticShadowVolumeShader::SetLightPos(const Vector3f& Pos)
{
    glUniform3f(m_lightPosLocation, Pos.x, Pos.y, Pos.z);
}