#include "testprojectile.h"

TestProjectile::TestProjectile() : m_visible(false)
{

}

TestProjectile::~TestProjectile()
{

}

void TestProjectile::Init()
{
	m_model.Load(MODEL_PATH "cube.lcm");
	m_model.SetPosition(GetPosition());
}

void TestProjectile::Move( float elapsedTime )
{
	Projectile::Move(elapsedTime);
	m_model.SetPosition(GetPosition());
}

void TestProjectile::Render() const {
	if (m_visible)
		m_model.Render(false);
}

void TestProjectile::Shoot()
{
	m_visible = true;
	Projectile::Shoot();
}

void TestProjectile::Hit()
{
	m_visible = false;
}

