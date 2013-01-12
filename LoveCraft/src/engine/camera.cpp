#include "camera.h"
#include "util/quaternion.h"

Camera::Camera( Vector3f position, Vector2f rotation) : m_pos(position), m_rot(rotation), m_mode(CAM_THIRD_PERSON), m_camRadius(10)
{

}

void Camera::TurnLeftRight ( float value )
{
	m_rot.y += value * MOUSE_SENSIBILITY;
}

void Camera::TurnTopBottom ( float value )
{
	//Assignation de la nouvelle rotation
	float newRotation = m_rot.x + value * MOUSE_SENSIBILITY;
	//Test de la rotation entre les limites établies
	if (newRotation >= -85.f && newRotation <= 85.f)
		m_rot.x = newRotation;
}

Vector2f Camera::GetRotation() const
{
	return m_rot;
}

void Camera::SetRotation( Vector2f rot )
{
	m_rot = rot;
}

void Camera::SetCamRadius(float radius)
{
	m_camRadius = radius;
}

Vector3f Camera::GetPosition() const
{
	return m_pos;
}

Vector3f Camera::GetRealPosition() const
{
	Vector3f realPos(0);
	realPos.z = m_camRadius;
	Quaternion qX;
	qX.FromAxis(PII * m_rot.x / 180, Vector3f(1,0,0));
	Quaternion qY;
	qY.FromAxis(PII * m_rot.y / 180, Vector3f(0,1,0));

	realPos = qX * realPos;
	realPos = qY * realPos;
	realPos *= m_camRadius;
	realPos += m_pos;
	realPos.Afficher();
	return realPos;
}

void Camera::SetPosition(Vector3f pos)
{
	m_pos = pos;
}

void Camera::ApplyRotation() const
{
	glRotatef(m_rot.x, 1.f, 0.f, 0.f);
	glRotatef(m_rot.y, 0.f, 1.f, 0.f);
}

void Camera::ApplyTranslation() const
{
	glTranslatef(-m_pos.x, -m_pos.y, -m_pos.z);
}

Camera::Mode Camera::GetMode() const
{
	return m_mode;
}

void Camera::SetMode( Mode mode )
{
	m_mode = mode;
}

Camera::~Camera()
{

}

