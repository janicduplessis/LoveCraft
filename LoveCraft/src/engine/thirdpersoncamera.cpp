#include "thirdpersoncamera.h"

ThirdPersonCamera::ThirdPersonCamera(Player* player, float distance) : m_player(player), m_distance(distance)
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float elapsedTime)
{
	if (!m_player)
		return;

	Vector3f distance = m_target * m_distance;
	m_pos = m_player->Position() - distance;
}


void ThirdPersonCamera::SetCamRadius( float m_camRadius )
{
	m_distance = m_camRadius;
}

void ThirdPersonCamera::MouseMoveEvent(const MouseEventArgs& e)
{
	const Vector3f vAxis(0.0f, 1.0f, 0.0f);
	// Rotate the view vector by the horizontal angle around the vertical axis
	m_target.Rotate(-e.GetDeltaPosition().x * MOUSE_SENSIBILITY, vAxis);
	m_target.Normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Vector3f hAxis = vAxis.Cross(m_target);
	hAxis.Normalize();
	m_target.Rotate(e.GetDeltaPosition().y * MOUSE_SENSIBILITY, hAxis);
	m_target.Normalize();

	m_up = m_target.Cross(hAxis);
	m_up.Normalize();

}