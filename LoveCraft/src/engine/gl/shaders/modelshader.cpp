#include "modelshader.h"
#include "util/tool.h"
#include <string>
#include <cassert>

static const char* pEffectFile = SHADER_PATH "model.glsl";

ModelShader::ModelShader() : ShaderNew(pEffectFile)
{

}

ModelShader::~ModelShader()
{

}

bool ModelShader::Init()
{
	if (!CompileProgram("Lighting"))
		return false;

	m_WVPLocation = GetUniformLocation("gWVP");
	m_worldLocation = GetUniformLocation("gWorld");
	m_colorSamplerLocation = GetUniformLocation("gColorSampler");
	//m_normalSamplerLocation = GetUniformLocation("gNormalSampler");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");

	// Valide les uniforms
	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_worldLocation == INVALID_UNIFORM_LOCATION ||
		m_colorSamplerLocation == INVALID_UNIFORM_LOCATION ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
		m_numPointLightsLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); ++i)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		_snprintf_s(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		// Valide les uniforms
		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
				return false;
		}
	}


	for (uint32 i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		_snprintf_s(Name, sizeof(Name), "gBones[%d]", i);
		m_boneLocation[i] = GetUniformLocation(Name);
	}


	return true;
}

void ModelShader::SetWVP( const Matrix4f& WVP )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.GetInternalValues());
}

void ModelShader::SetColorTextureUnit( unsigned int TextureUnit )
{
	glUniform1i(m_colorSamplerLocation, TextureUnit);
}

void ModelShader::SetDirectionalLight( const DirectionalLight& Light )
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	Vector3f direction = Light.Direction;
	direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void ModelShader::SetWorld( const Matrix4f& world )
{
	glUniformMatrix4fv(m_worldLocation, 1, GL_TRUE, (const GLfloat*)world.GetInternalValues());
}

void ModelShader::SetEyeWorldPos( const Vector3f& eyePos )
{
	glUniform3f(m_eyeWorldPosLocation, eyePos.x, eyePos.y, eyePos.z);
}

void ModelShader::SetMatSpecualarIntensity( float intensity )
{
	glUniform1f(m_matSpecularIntensityLocation, intensity);
}

void ModelShader::SetMatSpecularPower( float power )
{
	glUniform1f(m_matSpecularPowerLocation, power);
}

void ModelShader::SetPointLights( unsigned int numLights, const PointLight* pLights )
{
	glUniform1i(m_numPointLightsLocation, numLights);

	for (unsigned int i = 0; i < numLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void ModelShader::UpdatePointLight( unsigned int id, const PointLight& pLight )
{
	glUniform3f(m_pointLightsLocation[id].Color, pLight.Color.x, pLight.Color.y, pLight.Color.z);
	glUniform1f(m_pointLightsLocation[id].AmbientIntensity, pLight.AmbientIntensity);
	glUniform1f(m_pointLightsLocation[id].DiffuseIntensity, pLight.DiffuseIntensity);
	glUniform3f(m_pointLightsLocation[id].Position, pLight.Position.x, pLight.Position.y, pLight.Position.z);
	glUniform1f(m_pointLightsLocation[id].Atten.Constant, pLight.Attenuation.Constant);
	glUniform1f(m_pointLightsLocation[id].Atten.Linear, pLight.Attenuation.Linear);
	glUniform1f(m_pointLightsLocation[id].Atten.Exp, pLight.Attenuation.Exp);
}

void ModelShader::SetBoneTransform( uint32 index, const Matrix4f& transform )
{
	assert(index < MAX_BONES);
	glUniformMatrix4fv(m_boneLocation[index], 1, GL_TRUE, (const GLfloat*)transform.GetInternalValues()); 
}

