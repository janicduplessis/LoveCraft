#include "particles.h"
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <util/tool.h>
#include "../info.h"
#include "../camera.h"

#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

Particles::Particles(unsigned int particlesNumber) : m_particlesNumber(particlesNumber), 
	m_pos(0), m_range(0.5f), m_color(0.5f), m_angle(Quaternion(1,0,0,0)), m_averageVelocity(2),
	m_particlesSize(0.1f), m_averageLifespan(1.5), m_texture(0), m_isFirst(true), m_time(0), 
	m_currTFB(1), m_currVB(0)
{

}

//Particles::Particles( const Particles& p) : m_particlesNumber(p.m_particlesNumber),
//	m_pos(p.m_pos), m_range(p.m_range), m_color(p.m_color), m_angle(p.m_angle), 
//	m_averageVelocity(p.m_averageVelocity), m_averageLifespan(p.m_averageLifespan), 
//	m_particlesSize(p.m_particlesSize), m_texture(p.m_texture), m_randomTexture(p.m_randomTexture),
//	m_time(p.m_time), m_currVB(p.m_currVB), m_currTFB(p.m_currTFB), m_updateShader(p.m_updateShader),
//	m_billboardShader(p.m_billboardShader), m_isFirst(p.m_isFirst)
//{
//
//}

Particles::~Particles()
{
	if (m_transformFeedback[0] != 0) {
		glDeleteTransformFeedbacks(2, m_transformFeedback);
	}

	if (m_particleBuffer[0] != 0) {
		glDeleteBuffers(2, m_particleBuffer);
	}
}

bool Particles::Init()
{
	Particle* particles = new Particle[m_particlesNumber];

	particles[0].type = PARTICLE_TYPE_LAUNCHER;
	particles[0].pos = Vector3f(0);
	particles[0].velocity = Vector3f(0.f, 0.01f, 0.f);
	particles[0].lifespan = 0.f;

	glGenTransformFeedbacks(2, m_transformFeedback);
	glGenBuffers(2, m_particleBuffer);

	for (unsigned int i = 0; i < 2 ; i++) {
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * m_particlesNumber, particles, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_particleBuffer[i]);
	}

	delete [] particles;

	if (!m_updateShader.Init()) {
		return false;
	}

	m_updateShader.Use();

	m_updateShader.SetRandomTextureUnit(3);
	m_updateShader.SetLauncherLifetime(10.0f);
	m_updateShader.SetShellLifetime(3000.0f);
	m_updateShader.SetSecondaryShellLifetime(2000.0f);
	m_updateShader.SetLauncherPosition(m_pos);

	//Shader::Disable();

	if (!m_randomTexture.InitRandomTexture(1000)) {
		return false;
	}

	if (!m_billboardShader.Init()) {
		return false;
	}

	m_billboardShader.Use();

	//m_billboardTechnique.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

	m_billboardShader.SetBillboardSize(0.02f);

	//Shader::Disable();

	m_texture = new Texture;

	if (!m_texture->Load(TEXTURE_PATH "particle1.png")) {
		return false;
	}  
	return true;
}

void Particles::Update(int deltaTimeMilli)
{
	m_time += deltaTimeMilli;

	m_updateShader.Use();
	m_updateShader.SetTime(m_time);
	m_updateShader.SetDeltaTimeMillis(deltaTimeMilli);

	m_randomTexture.Bind(GL_TEXTURE3);
	CHECK_GL_ERROR();

	glEnable(GL_RASTERIZER_DISCARD);
	CHECK_GL_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currVB]);    
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[m_currTFB]);
	CHECK_GL_ERROR();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	CHECK_GL_ERROR();

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);                          // type
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);         // position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)16);        // velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)28);          // lifetime
	CHECK_GL_ERROR();

	glBeginTransformFeedback(GL_POINTS);
	CHECK_GL_ERROR();

	if (m_isFirst) {
		glDrawArrays(GL_POINTS, 0, 1);

		m_isFirst = false;
	}
	else {
		glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currVB]);
	}            

	glEndTransformFeedback();
	CHECK_GL_ERROR();

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	CHECK_GL_ERROR();

	//Shader::Disable();
}

void Particles::Render(const Matrix4f& VP, bool wireFrame)
{
	Shader::Disable();
	m_billboardShader.Use();
	m_billboardShader.SetCameraPosition(Info::Get().GetCamera()->GetPosition());
	m_billboardShader.SetVP(VP);
	m_texture->Bind();

	glDisable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[m_currTFB]);    

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);  // position

	glDrawTransformFeedback(GL_POINTS, m_transformFeedback[m_currTFB]);

	glDisableVertexAttribArray(0);

	m_currVB = m_currTFB;
	m_currTFB = (m_currTFB + 1) & 0x1;

	Shader::Disable();
}

Vector3f Particles::AvgVelocity() const
{
	return m_angle * Vector3f(-1,0,0) * m_averageVelocity;
}


void Particles::SetPosition( const Vector3f& pos )
{
	m_pos = pos;
	m_updateShader.Use();
	m_updateShader.SetLauncherPosition(m_pos);
}

void Particles::SetRotation( const Quaternion& q )
{
	m_angle = q;
}

void Particles::SetParticlesNumber( float nbr )
{
	m_particlesNumber = nbr;
}

void Particles::SetRange( float range )
{
	m_range = range;
}

void Particles::SetParticlesSize( float size )
{
	m_particlesSize = size;
}

void Particles::SetTexture( Texture* texture )
{
	m_texture = texture;
}

void Particles::SetColor( const Vector3f& color )
{
	m_color = color;
}

void Particles::SetAverageLifespan( float lifespan )
{
	m_averageLifespan = lifespan;
}

void Particles::SetAverageVelocity( float velocity )
{
	m_averageVelocity = velocity;
}

