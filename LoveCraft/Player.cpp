#include "Player.h"
#include "openglcontext.h"
#include <cmath>
#include <iostream>

Player::Player(Vector3f position, Vector2f rotation)
	:m_pos(position), m_rot(rotation)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_model.Load("../lovecraft/models/monkey.obj");
}

void Player::TurnLeftRight ( float value )
{
	m_rot.y += value * MOUSE_SENSIBILITY;
}

void Player::TurnTopBottom ( float value )
{
	//Assignation de la nouvelle rotation
	float newRotation = m_rot.x + value * MOUSE_SENSIBILITY;
	//Test de la rotation entre les limites établies
	if (newRotation >= -85.f && newRotation <= 85.f)
		m_rot.x = newRotation;
}

void Player::Move ( bool front , bool back , bool left , bool right , bool run, bool ghost, float elapsedTime )
{
	if (front)
	{
		float xRotRad, yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		xRotRad = (m_rot.x / 180 * PII);
		m_pos.x += float(sin(yRotRad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		m_pos.z -= float(cos(yRotRad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		if (ghost)
			m_pos.y -= float(sin(xRotRad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
	}

	if (back)
	{
		float xRotRad, yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		xRotRad = (m_rot.x / 180 * PII);
		m_pos.x -= float(sin(yRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		m_pos.z += float(cos(yRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		if (ghost)
			m_pos.y += float(sin(xRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
	}
	if (right)
	{
		float yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		m_pos.x += float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_pos.z += float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}

	if (left)
	{
		float yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		m_pos.x -= float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_pos.z -= float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}
}

void Player::DrawModel() const 
{
	m_model.Render();
}

Vector3f Player::GetPosition() const
{
	return m_pos;
}

Vector2f Player::GetRotation() const
{
	return m_rot;
}
