﻿#include "spell.h"


Spell::Spell() : m_particles(2000), m_spellTexture(0)
{
	
}

Spell::Spell( const Spell& s ) : m_particles(s.m_particles), m_spellTexture(s.m_spellTexture), Projectile(s)
{
	m_particles.SetTexture(m_spellTexture);
}

Spell::~Spell()
{

}

void Spell::Update( float elapsedTime )
{
	Projectile::Update(elapsedTime);
	m_particles.Update(elapsedTime * 1000);
	m_particles.SetPosition(m_pos);
	m_particles.SetRotation(m_rot);
}

void Spell::Render(Matrix4f VP)
{
	m_particles.Render(VP);
}

void Spell::SetPosition( const Vector3f& pos )
{
	Projectile::SetPosition(pos);
	m_particles.SetPosition(pos);
}

void Spell::Init( float speed, const Quaternion& rot)
{
	Projectile::Init(speed, rot);
	m_particles.SetRotation(rot);
	m_spellTexture = new Texture;
	m_spellTexture->Load(TEXTURE_PATH "particle1.png");
	m_particles.SetTexture(m_spellTexture);
	m_particles.SetColor(Vector3f(1,0,0));
	m_particles.SetParticlesSize(0.1);
	m_particles.SetAverageVelocity(0.01);
	m_particles.SetColor(Vector3f(1, 153/255.f, 0));
	m_particles.SetRange(0.2);
	m_particles.Init();
}


