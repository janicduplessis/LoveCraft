#include "player.h"
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
	m_model.Load(MODEL_PATH "monkey.lcm");
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

void Player::Move(Array<bool>& controls, bool ghost, float elapsedTime )
{
	if (controls.Get(22))	// W
	{
		float xRotRad, yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		xRotRad = (m_rot.x / 180 * PII);
		m_pos.x += float(sin(yRotRad)) * (controls.Get(38) ? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		m_pos.z -= float(cos(yRotRad)) * (controls.Get(38) ? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		if (ghost)
			m_pos.y -= float(sin(xRotRad)) * (controls.Get(38) ? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
	}

	if (controls.Get(18))	// S
	{
		float xRotRad, yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		xRotRad = (m_rot.x / 180 * PII);
		m_pos.x -= float(sin(yRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		m_pos.z += float(cos(yRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		if (ghost)
			m_pos.y += float(sin(xRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
	}
	if (controls.Get(3))	// D
	{
		float yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		m_pos.x += float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_pos.z += float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}

	if (controls.Get(0))	// A
	{
		float yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		m_pos.x -= float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_pos.z -= float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}
	if (controls.Get(57))	// Space
	{
		if (ghost)
			m_pos.y += MOUVEMENT_SPEED * (1.f + elapsedTime);
		else
		{
			//Code du jump ici
		}
	}
	if (controls.Get(37))	// Ctrl
	{
		if (ghost)
			m_pos.y -= MOUVEMENT_SPEED * (1.f + elapsedTime);
		else
		{
			//Code du crouch ici
		}
	}
}

void Player::Render(bool wireFrame)
{
	m_model.SetPosition(m_pos);
	m_model.SetRotation(Vector3f(0, m_rot.y, 0));
	m_model.Render(wireFrame);
}

Vector3f Player::Position() const
{
	return m_pos;
}

Vector2f Player::Rotation() const
{
	return m_rot;
}

void Player::SetRotation( Vector2f rot )
{
	m_rot = rot;
}
