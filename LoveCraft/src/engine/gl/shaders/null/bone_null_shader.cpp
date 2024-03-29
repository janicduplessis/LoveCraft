#include "bone_null_shader.h"

static const char* shaderFile = SHADER_PATH "null/null.glsl";

BoneNullShader::BoneNullShader() : Shader(shaderFile)
{   
}

bool BoneNullShader::Init()
{
    if (!CompileProgram("BoneNullShader")) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
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


void BoneNullShader::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

void BoneNullShader::SetBoneTransform( uint32 index, const Matrix4f& transform )
{
	assert(index < MAX_BONES);
	glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, (const GLfloat*)transform.GetInternalValues()); 
}

