#include "testprojectile.h"

TestProjectile::TestProjectile()
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
	m_model.Render(false);
}

