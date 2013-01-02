#include "testprojectile.h"

TestProjectile::TestProjectile() : m_visible(false)
{

}

TestProjectile::~TestProjectile()
{

}

void TestProjectile::Load()
{
	m_model.Load(MODEL_PATH "cube.lcm");
	m_model.SetPosition(GetPosition());
}

void TestProjectile::Move( float elapsedTime )
{
	if(m_visible) {
		Projectile::Move(elapsedTime);
		m_model.SetPosition(GetPosition());
		m_model.SetRotation(m_rot.GetConjugate());
	}
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

