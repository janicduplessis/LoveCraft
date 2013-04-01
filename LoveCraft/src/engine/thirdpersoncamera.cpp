#include "thirdpersoncamera.h"

ThirdPersonCamera::ThirdPersonCamera(Player* player, float distance) : m_player(player), m_distance(distance)
{
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float elapsedTime)
{
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
	Vector3f view(1.0f, 0.0f, 0.0f);
	view.Rotate(e.GetDeltaPosition().x, vAxis);
	view.Normalize();

	// Rotate the view vector by the vertical angle around the horizontal axis
	Vector3f hAxis = vAxis.Cross(view);
	hAxis.Normalize();
	view.Rotate(e.GetDeltaPosition().y, hAxis);

	m_target = view;
	m_target.Normalize();

	m_up = m_target.Cross(hAxis);
	m_up.Normalize();

}


