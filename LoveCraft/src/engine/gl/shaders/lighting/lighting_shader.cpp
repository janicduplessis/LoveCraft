#include "lighting_shader.h"
#include "util/tool.h"
#include <string>
#include "glfx.h"

static const char* pEffectFile = SHADER_PATH "lighting/lighting.glsl";

LightingShader::LightingShader() : Shader(pEffectFile)
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

