#include <glfx.h>

#include "bone_shadow_volume_shader.h"

static const char* pEffectFile = SHADER_PATH "shadow/shadow_volume.glsl";

BoneShadowVolumeShader::BoneShadowVolumeShader() : Shader(pEffectFile)
{   
}

bool BoneShadowVolumeShader::Init()
{
    if (!CompileProgram("BoneShadowVolume")) {
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

	for (uint32 i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gBones[%d]", i);
		m_boneLocation[i] = GetUniformLocation(Name);
	}
            
    return true;
}

void BoneShadowVolumeShader::SetVP(const Matrix4f& VP)
{
    glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)VP.m);    
}


void BoneShadowVolumeShader::SetWorldMatrix(const Matrix4f& WorldInverse)
{
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
}

void BoneShadowVolumeShader::SetLightPos(const Vector3f& Pos)
{
    glUniform3f(m_lightPosLocation, Pos.x, Pos.y, Pos.z);
}

void BoneShadowVolumeShader::SetBoneTransform( uint32 index, const Matrix4f& transform )
{
	assert(index < MAX_BONES);
	glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, (const GLfloat*)transform.GetInternalValues()); 
}