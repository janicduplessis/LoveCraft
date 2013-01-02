#ifndef SPELL_H__
#define SPELL_H__

#include <engine/projectile.h>
#include <engine/gl/particles.h>
#include <engine/gl/texture.h>

class Spell : public Projectile
{
public:
	Spell();
	virtual ~Spell();

	virtual void Move(float elapsedTime);

	virtual void SetPosition(const Vector3f& pos);

private:
	Particles m_particles;
	Texture m_spellTexture;
};

#endif