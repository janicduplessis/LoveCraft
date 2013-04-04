#include "lightingshader.h"
#include "util/tool.h"
#include <string>
#include "glfx.h"

static const char* pEffectFile = SHADER_PATH "lighting.glsl";

LightingShader::LightingShader() : ShaderNew(pEffectFile)
{

}

LightingShader::~LightingShader()
{

}

bool LightingShader::Init()
{
	if (!CompileProgram("Lighting"))
		return false;

	glfxGenerateSampler(m_effect, "Sampler");
	m_WVPLocation = GetUniformLocation("gWVP");
	m_worldLocation = GetUniformLocation("gWorld");
	m_samplerTypeLocation = GetUniformLocation("gSamplerType");
	m_arraySamplerLocation = GetUniformLocation("gArraySampler");
	/*m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gMatSpecularPower");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");/*
	/*
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

	// Valide les uniforms
	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		m_worldLocation == INVALID_UNIFORM_LOCATION ||
		m_colorSamplerLocation == INVALID_UNIFORM_LOCATION ||
		m_samplerTypeLocation == INVALID_UNIFORM_LOCATION ||
		m_arraySamplerLocation == INVALID_UNIFORM_LOCATION ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
		m_numPointLightsLocation == INVALID_UNIFORM_LOCATION) {
			return false;
	}*/

	return true;
}

void LightingShader::SetWVP( const Matrix4f& WVP )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.GetInternalValues());
}

void LightingShader::SetColorTextureUnit( unsigned int TextureUnit )
{
	glUniform1i(m_arraySamplerLocation, TextureUnit + 1);
}

void LightingShader::SetDirectionalLight( const DirectionalLight& Light )
{
	/*glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	Vector3f direction = Light.Direction;
	direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);*/
}

void LightingShader::SetWorld( const Matrix4f& world )
{
	glUniformMatrix4fv(m_worldLocation, 1, GL_TRUE, (const GLfloat*)world.GetInternalValues());
}

void LightingShader::SetEyeWorldPos( const Vector3f& eyePos )
{
	//glUniform3f(m_eyeWorldPosLocation, eyePos.x, eyePos.y, eyePos.z);
}

void LightingShader::SetMatSpecualarIntensity( float intensity )
{
	//glUniform1f(m_matSpecularIntensityLocation, intensity);
}

void LightingShader::SetMatSpecularPower( float power )
{
	//glUniform1f(m_matSpecularPowerLocation, power);
}

void LightingShader::SetPointLights( unsigned int numLights, const PointLight* pLights )
{
	/*glUniform1i(m_numPointLightsLocation, numLights);

	for (unsigned int i = 0; i < numLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}*/
}

void LightingShader::SetTextureUnitType( int type )
{
	glUniform1i(m_samplerTypeLocation, type);
}

void LightingShader::UpdatePointLight( unsigned int id, const PointLight& pLight )
{
	/*glUniform3f(m_pointLightsLocation[id].Color, pLight.Color.x, pLight.Color.y, pLight.Color.z);
	glUniform1f(m_pointLightsLocation[id].AmbientIntensity, pLight.AmbientIntensity);
	glUniform1f(m_pointLightsLocation[id].DiffuseIntensity, pLight.DiffuseIntensity);
	glUniform3f(m_pointLightsLocation[id].Position, pLight.Position.x, pLight.Position.y, pLight.Position.z);
	glUniform1f(m_pointLightsLocation[id].Atten.Constant, pLight.Attenuation.Constant);
	glUniform1f(m_pointLightsLocation[id].Atten.Linear, pLight.Attenuation.Linear);
	glUniform1f(m_pointLightsLocation[id].Atten.Exp, pLight.Attenuation.Exp);*/
}

