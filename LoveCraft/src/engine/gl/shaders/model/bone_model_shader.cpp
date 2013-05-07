#include "bone_model_shader.h"
#include "util/tool.h"
#include <string>
#include <cassert>
#include "glfx.h"

static const char* pEffectFile = SHADER_PATH "model/model.glsl";

BoneModelShader::BoneModelShader() : Shader(pEffectFile)
{

}

BoneModelShader::~BoneModelShader()
{

}

bool BoneModelShader::Init()
{
	if (!CompileProgram("BoneModelGeometryPass"))
		return false;

	glfxGenerateSampler(m_effect, "Sampler");
	m_WVPLocation = GetUniformLocation("gWVP");
	m_worldLocation = GetUniformLocation("gWorld");

	// Valide les uniforms
	if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_worldLocation == INVALID_UNIFORM_LOCATION)
			return false;

	for (uint32 i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gBones[%d]", i);
		m_boneLocation[i] = GetUniformLocation(Name);
	}

	return true;
}

void BoneModelShader::SetWVP( const Matrix4f& WVP )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.GetInternalValues());
}

void BoneModelShader::SetWorld( const Matrix4f& world )
{
	glUniformMatrix4fv(m_worldLocation, 1, GL_TRUE, (const GLfloat*)world.GetInternalValues());
}

void BoneModelShader::SetBoneTransform( uint32 index, const Matrix4f& transform )
{
	assert(index < MAX_BONES);
	glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, (const GLfloat*)transform.GetInternalValues()); 
}

