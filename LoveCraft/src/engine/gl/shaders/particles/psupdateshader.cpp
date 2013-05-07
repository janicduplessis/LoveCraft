#include "psupdateshader.h"
#include <iostream>


PSUpdateShader::PSUpdateShader() : Shader("yo")
{

}

PSUpdateShader::~PSUpdateShader()
{

}

bool PSUpdateShader::Init()
{
	/*std::cout << "Update Shader :" << std::endl;
	Shader::Init();
	AddShader(GL_VERTEX_SHADER_ARB, SHADER_PATH "psupdateshader.vert", true);
	AddShader(GL_GEOMETRY_SHADER_ARB, SHADER_PATH "psupdateshader.geom", true);

	const GLchar* Varyings[5];    
	Varyings[0] = "Type1";
	Varyings[1] = "Position1";
	Varyings[2] = "Velocity1";    
	Varyings[3] = "Age1";
	Varyings[4] = "Alpha1";

	glTransformFeedbackVaryings(m_program, 5, Varyings, GL_INTERLEAVED_ATTRIBS);

	Link();

	m_deltaTimeMillisLocation = BindUniform("gDeltaTimeMillis");
	m_randomTextureLocation = BindUniform("gRandomTexture");
	m_timeLocation = BindUniform("gTime");
	m_launcherLifetimeLocation = BindUniform("gLauncherLifetime");
	m_shellLifetimeLocation = BindUniform("gShellLifetime");
	m_secondaryShellLifetimeLocation = BindUniform("gSecondaryShellLifetime");
	m_launcherPosLocation = BindUniform("gLauncherPosition");
	m_rangeLocation = BindUniform("gRange");
	m_avgVelocityLocation = BindUniform("gAvgVelocity");
	*/
	return true;
}

void PSUpdateShader::SetDeltaTimeMillis( int deltaTimeMilli )
{
	glUniform1f(m_deltaTimeMillisLocation, (float)deltaTimeMilli);
}

void PSUpdateShader::SetTime( int time )
{
	glUniform1f(m_timeLocation, (float)time);
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

void PSUpdateShader::SetLauncherPosition( Vector3f pos )
{
	glUniform3f(m_launcherPosLocation, pos.x, pos.y, pos.z);
}

void PSUpdateShader::SetAvgVelocity( const Vector3f& vel )
{
	glUniform3f(m_avgVelocityLocation, vel.x, vel.y, vel.z);
}

void PSUpdateShader::SetRange( float range )
{
	glUniform1f(m_rangeLocation, range);
}

