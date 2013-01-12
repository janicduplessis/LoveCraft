#include "psupdateshader.h"
#include <iostream>


PSUpdateShader::PSUpdateShader()
{

}

PSUpdateShader::~PSUpdateShader()
{

}

bool PSUpdateShader::Init()
{
	std::cout << "Update Shader :" << std::endl;
	Shader::Init();
	AddShader(GL_VERTEX_SHADER_ARB, SHADER_PATH "psupdateshader.vert", true);
	AddShader(GL_GEOMETRY_SHADER_ARB, SHADER_PATH "psupdateshader.geom", true);
	Link();

	const GLchar* Varyings[4];    
	Varyings[0] = "Type1";
	Varyings[1] = "Position1";
	Varyings[2] = "Velocity1";    
	Varyings[3] = "Age1";

	glTransformFeedbackVaryings(m_program, 4, Varyings, GL_INTERLEAVED_ATTRIBS);

	m_deltaTimeMillisLocation = BindUniform("gDeltaTimeMillis");
	m_randomTextureLocation = BindUniform("gRandomTexture");
	m_timeLocation = BindUniform("gTime");
	m_launcherLifetimeLocation = BindUniform("gLauncherLifetime");
	m_shellLifetimeLocation = BindUniform("gShellLifetime");
	m_secondaryShellLifetimeLocation = BindUniform("gSecondaryShellLifetime");

	return true;
}

void PSUpdateShader::SetDeltaTimeMillis( float elapsedTime )
{
	glUniform1f(m_deltaTimeMillisLocation, elapsedTime);
}

void PSUpdateShader::SetTime( float time )
{
	glUniform1f(m_timeLocation, time);
}

void PSUpdateShader::SetRandomTextureUnit( unsigned int textureUnit )
{
	glUniform1i(m_randomTextureLocation, textureUnit);
}

void PSUpdateShader::SetLauncherLifetime( float lifetime )
{
	glUniform1f(m_launcherLifetimeLocation, lifetime);
}

void PSUpdateShader::SetShellLifetime( float lifetime )
{
	glUniform1f(m_shellLifetimeLocation, lifetime);
}

void PSUpdateShader::SetSecondaryShellLifetime( float lifetime )
{
	glUniform1f(m_secondaryShellLifetimeLocation, lifetime);
}

