#include "quaternion.h"
#include <iostream>


Quaternion::Quaternion( float w, float x, float y, float z ) :
	m_w(w), m_x(x), m_y(y), m_z(z)
{

}

void Quaternion::Normalize()
{
	float magnitude = sqrt(Magnitude());
	m_w /= magnitude;
	m_x /= magnitude;
	m_y /= magnitude;
	m_z /= magnitude;
}

float Quaternion::Magnitude()
{
	// magnitude = sqrt(w^2 + x^2 + y^2 + z^2)
	return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
}

Matrix4f Quaternion::RotationMatrix()
{
	return Matrix4f(
		1 - 2 * m_y * m_y - 2 * m_z * m_z,	2 * m_x * m_y - 2 * m_w * m_z,		2 * m_x * m_z + 2 * m_w * m_y,		0,
		2 * m_x * m_y + 2 * m_w * m_z,		1 - 2 * m_x * m_x - 2 * m_z * m_z,	2 * m_y * m_z + 2 * m_w * m_x,		0,
		2 * m_x * m_z - 2 * m_w * m_y,		2 * m_y * m_z - 2 * m_w * m_x,		1 - 2 * m_x * m_x - 2 * m_y * m_y,	0,
		0,									0,									0,									1 );
}

Quaternion Quaternion::operator*( const Quaternion& q ) const
{
	Quaternion tmp;

	tmp.m_w = (q.m_w * m_w) - (q.m_x * m_x) - (q.m_y * m_y) - (q.m_z * m_z);
	tmp.m_x = (q.m_w * m_x) + (q.m_x * m_w) + (q.m_y * m_z) - (q.m_z * m_y);
	tmp.m_y = (q.m_w * m_y) + (q.m_y * m_w) + (q.m_z * m_x) - (q.m_x * m_z);
	tmp.m_z = (q.m_w * m_z) + (q.m_z * m_w) + (q.m_x * m_y) - (q.m_y * m_x);

	return tmp;
}

void Quaternion::SetRotation( float angle, Vector3f axis )
{
	/*m_w = cosf(-angle / 2);
	m_x = axis.x * sinf(angle / 2);
	m_y = axis.y * sinf(angle / 2);
	m_z = axis.z * sinf(angle / 2);
	Normalize();*/
	float omega, s, c;

	s = sqrt(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);

	c = 1.0/s;

	axis.x *= c;
	axis.y *= c;
	axis.z *= c;

	omega = -0.5f * angle;
	s = (float)sin(omega);

	m_x = s*axis.x;
	m_y = s*axis.y;
	m_z = s*axis.z;
	m_w = (float)cos(omega);

	Normalize();
}

void Quaternion::Afficher() const
{
	std::cout << m_w << std::endl;
	std::cout << m_x << std::endl;
	std::cout << m_y << std::endl;
	std::cout << m_z << std::endl;
}

