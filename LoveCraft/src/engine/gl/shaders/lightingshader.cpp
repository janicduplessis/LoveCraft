#include "lightingshader.h"
#include "util/tool.h"

LightingShader::LightingShader()
{

}

LightingShader::~LightingShader()
{

}

bool LightingShader::Init()
{
	Shader::Init();
	AddShader(GL_VERTEX_SHADER, SHADER_PATH "lightingshader.vert", true);
	AddShader(GL_FRAGMENT_SHADER, SHADER_PATH "lightingshader.frag", true);
	Link();

	m_WVPLocation = BindUniform("gWVP");
	m_worldLocation = BindUniform("gWorld");
	m_samplerLocation = BindUniform("gSampler");
	m_dirLightLocation.Color = BindUniform("gDirectionalLight.Color");
	m_dirLightLocation.AmbientIntensity = BindUniform("gDirectionalLight.AmbientIntensity");
	m_dirLightLocation.DiffuseIntensity = BindUniform("gDirectionalLight.DiffuseIntensity");
	m_dirLightLocation.Direction = BindUniform("gDirectionalLight.Direction");
	m_matSpecularIntensityLocation = BindUniform("gMatSpecularIntensity");
	m_matSpecularPowerLocation = BindUniform("gMatSpecularPower");
	m_eyeWorldPosLocation = BindUniform("gEyeWorldPos");

	return true;
}

void LightingShader::SetWVP( const Matrix4f& WVP )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.GetInternalValues());
}

void LightingShader::SetTextureUnit( unsigned int TextureUnit )
{
	glUniform1i(m_samplerLocation, TextureUnit);
}

void LightingShader::SetDirectionalLight( const DirectionalLight& Light )
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	Vector3f direction = Light.Direction;
	direction.Normalise();
	glUniform3f(m_dirLightLocation.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingShader::SetWorld( const Matrix4f& world )
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)world.GetInternalValues());
}

void LightingShader::SetEyeWorldPos( const Vector3f& eyePos )
{
	glUniform3f(m_eyeWorldPosLocation, eyePos.x, eyePos.y, eyePos.z);
}

void LightingShader::SetMatSpecualarIntensity( float intensity )
{
	glUniform1f(m_matSpecularIntensityLocation, intensity);
}

void LightingShader::SetMatSpecularPower( float power )
{
	glUniform1f(m_matSpecularPowerLocation, power);
}

