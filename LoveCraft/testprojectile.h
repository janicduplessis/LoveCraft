#ifndef TESTPROJ_H__
#define TESTPROJ_H__

#include "projectile.h"
#include "model.h"

class TestProjectile : public Projectile
{
public:
	TestProjectile();
	~TestProjectile();

	void Init();
	void Move(float elapsedTime);
	void Shoot();
	void Hit();
	void Render() const;
private:
	Model m_model;
	bool m_visible;
};

#endif