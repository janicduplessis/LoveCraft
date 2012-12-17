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

void TestProjectile::TestRotation()
{
	Projectile::TestRotation();
	m_model.SetPosition(GetPosition());
}

void TestProjectile::Render() const {
	m_model.Render(false);
	glPushMatrix();
	Vector3f& v = GetDestination();
	glTranslatef(v.x, v.y, v.z);
	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0);
	glVertex3f(0.5, 0.0, 0.5);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glEnd();

	glPopMatrix();
}

