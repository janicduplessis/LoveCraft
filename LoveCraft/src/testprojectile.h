#ifndef TESTPROJ_H__
#define TESTPROJ_H__

#include "engine/projectile.h"
#include "engine/gl/model.h"

class TestProjectile : public Projectile
{
public:
	TestProjectile();
	~TestProjectile();

	void Load();
	void Move(float elapsedTime);
	void Shoot();
	void Hit();
	void Render() const;
private:
	Model m_model;
	bool m_visible;
};

#endif