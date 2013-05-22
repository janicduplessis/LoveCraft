#include <glfx.h>

#include "bone_shadow_map_shader.h"

static const char* shaderFile = SHADER_PATH "shadow/shadow_map.glsl";

BoneShadowMapShader::BoneShadowMapShader() : Shader(shaderFile)
{

}

bool BoneShadowMapShader::Init()
{
	if (!CompileProgram("BoneShadowMap")) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");
	m_textureLocation = GetUniformLocation("gShadowMap");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_textureLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	for (uint32 i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) {
		char name[128];
		memset(name, 0, sizeof(name));
		_snprintf_s(name, sizeof(name), "gBones[%d]", i);
		m_boneLocation[i] = GetUniformLocation(name);
	}

	return true;
}

void BoneShadowMapShader::SetWVP( const Matrix4f& wvp )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)wvp.m);
}

void BoneShadowMapShader::SetBoneTransform( uint32 index, const Matrix4f& transform )
{
	assert(index < MAX_BONES);
	glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, (const GLfloat*)transform.GetInternalValues()); 
}

void BoneShadowMapShader::SetTextureUnit( unsigned int textureUnit )
{
	glUniform1i(m_textureLocation, textureUnit);
}

