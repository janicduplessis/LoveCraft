#include "spell.h"


Spell::Spell() : m_particles(200)
{
	m_spellTexture.Load(TEXTURE_PATH "particle1.png");
	m_particles.SetTexture(&m_spellTexture);
	m_particles.Init();
	m_particles.SetColor(Vector3f(1,0,0));
}

Spell::~Spell()
{

}

void Spell::Update( float elapsedTime )
{
	Projectile::Update(elapsedTime);
	m_particles.Update(elapsedTime);
	m_particles.SetPosition(m_pos);
	m_particles.SetRotation(m_rot);
}

void Spell::Render() const
{
	m_particles.Render();
}

void Spell::SetPosition( const Vector3f& pos )
{
	Projectile::SetPosition(pos);
	m_particles.SetPosition(pos);
}

void Spell::Init( float speed, const Quaternion& rot )
{
	Projectile::Init(speed, rot);
	m_particles.SetRotation(rot);
}


