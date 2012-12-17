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
	void Render() const;
	void TestRotation();
private:
	Model m_model;
};

#endif