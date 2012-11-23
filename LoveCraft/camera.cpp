#include "camera.h"

Camera::Camera( Vector3f position, Vector2f rotation) : m_pos(position), m_rot(rotation)
{

}

Vector2f Camera::GetRotation() const
{
	return m_rot;
}

void Camera::SetRotation( Vector2f rot )
{
	m_rot = rot;
}

Vector3f Camera::GetPosition() const
{
	return m_pos;
}

void Camera::SetPosition( Vector3f pos )
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
	glTranslatef(- m_pos.x, - m_pos.y, - m_pos.z);
}

Camera::~Camera()
{

}

