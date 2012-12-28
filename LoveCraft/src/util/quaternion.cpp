#include "quaternion.h"
#include <iostream>


Quaternion::Quaternion( float w, float x, float y, float z ) :
	m_w(w), m_x(x), m_y(y), m_z(z)
{

}

void Quaternion::Normalise()
{
	float mag2 = Magnitude();
	if (fabs(mag2) > TOLERANCE && fabs(mag2 - 1.0f) > TOLERANCE) {
		float mag = sqrt(mag2);
		m_w /= mag;
		m_x /= mag;
		m_y /= mag;
		m_z /= mag;
	}
}

Quaternion Quaternion::GetConjugate() const
{
	return Quaternion(m_w, -m_x, -m_y, -m_z);
}

float Quaternion::Magnitude()
{
	// magnitude = sqrt(w^2 + x^2 + y^2 + z^2)
	return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
}

Matrix4f Quaternion::RotationMatrix() const
{
	float x2 = m_x * m_x;
	float y2 = m_y * m_y;
	float z2 = m_z * m_z;
	float xy = m_x * m_y;
	float xz = m_x * m_z;
	float yz = m_y * m_z;
	float wx = m_w * m_x;
	float wy = m_w * m_y;
	float wz = m_w * m_z;

	return Matrix4f( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
		2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
		2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

Quaternion Quaternion::operator* ( const Quaternion& q ) const
{
	return Quaternion(q.m_w * m_w - q.m_x * m_x - q.m_y * m_y - q.m_z * m_z,
					  q.m_w * m_x + q.m_x * m_w + q.m_y * m_z - q.m_z * m_y,
					  q.m_w * m_y + q.m_y * m_w + q.m_z * m_x - q.m_x * m_z,
					  q.m_w * m_z + q.m_z * m_w + q.m_x * m_y - q.m_y * m_x);
}

Vector3f Quaternion::operator* ( const Vector3f &vec ) const
{
	Vector3f vn = vec;
	vn.Normalise();

	Quaternion vecQuat, resQuat;
	vecQuat.m_x = vn.x;
	vecQuat.m_y = vn.y;
	vecQuat.m_z = vn.z;
	vecQuat.m_w = 0.0f;

	resQuat = vecQuat * GetConjugate();
	resQuat = *this * resQuat;

	return (Vector3f(resQuat.m_x, resQuat.m_y, resQuat.m_z));
}

void Quaternion::FromAxis( float angle, const Vector3f& axis )
{
	float sinAngle;
	angle *= 0.5f;
	Vector3f vn = axis;
	vn.Normalise();

	sinAngle = sin(angle);

	m_x = (vn.x * sinAngle);
	m_y = (vn.y * sinAngle);
	m_z = (vn.z * sinAngle);
	m_w = cos(angle);
}

void Quaternion::Afficher() const
{
	std::cout << m_w << " + ";
	std::cout << m_x << "i + ";
	std::cout << m_y << "j + ";
	std::cout << m_z << "k" << std::endl;
}

