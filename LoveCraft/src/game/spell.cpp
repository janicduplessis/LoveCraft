#include "spell.h"


Spell::Spell() : m_particles(200)
{
	m_spellTexture.Load(TEXTURE_PATH "particle1.png");
	m_particles.SetTexture(&m_spellTexture);
	m_particles.Init();
}

Spell::~Spell()
{

}

void Spell::Move( float elapsedTime )
{
	Projectile::Move(elapsedTime);
	m_particles.Update(elapsedTime);
	m_particles.Render();
}

void Spell::SetPosition( const Vector3f& pos )
{
	Projectile::SetPosition(pos);
	m_particles.SetPosition(pos);
}


