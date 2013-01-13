#ifndef SPELL_H__
#define SPELL_H__

#include <engine/projectile.h>
#include <engine/gl/particles.h>
#include <engine/gl/texture.h>

class Spell : public Projectile
{
public:
	Spell();
	Spell(const Spell& s);
	virtual ~Spell();

	virtual void Update(float elapsedTime);

	virtual void SetPosition(const Vector3f& pos);
	void Render(Matrix4f VP);

	virtual void Init( float speed, const Quaternion& rot);

private:
	Particles m_particles;
	Texture* m_spellTexture;
};

#endif