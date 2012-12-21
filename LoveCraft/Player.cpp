#include "player.h"
#include "openglcontext.h"
#include "son.h"
#include "info.h"
#include <cmath>
#include <iostream>

Player::Player(Vector3f position, Vector2f rotation)
	:m_pos(position), m_rot(rotation), m_speed(Vector3f()), 
	m_accel(Vector3f(MOUVEMENT_ACCELERATION, MOUVEMENT_ACCELERATION, MOUVEMENT_ACCELERATION))
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
	//Calcul de la position en fonction de la gravité
	if (!ghost)
	{
		float distance = (m_speed.y * elapsedTime) + (MOUVEMENT_ACCELERATION_DOWN * MOUVEMENT_WEIGHT_RATIO * elapsedTime * elapsedTime / 2.0f);
		if (Position().y - distance > 0)
		{
			m_pos.y -= distance;
			m_speed.y = m_speed.y + (MOUVEMENT_ACCELERATION_DOWN * MOUVEMENT_WEIGHT_RATIO * elapsedTime);
		}
		else
		{
			if (m_speed.y != 0)
				Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
			m_speed.y = 0;
			m_pos.y = 0;
		}
	}
	if (controls.Get(22))	// W
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);

		// Regarde si la vitesse dépasse la vitesse max
		if (m_speed.z >= MOUVEMENT_SPEED) {
			m_speed.z = MOUVEMENT_SPEED;
			m_accel.z = 0;
		}
		else m_accel.z = MOUVEMENT_ACCELERATION;

		float distance = (m_speed.z * elapsedTime) + (MOUVEMENT_ACCELERATION * elapsedTime * elapsedTime / 2.0f);
		m_speed.z = m_speed.z + (MOUVEMENT_ACCELERATION * elapsedTime);

		float xRotRad, yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		xRotRad = (m_rot.x / 180 * PII);
		m_pos.x += float(sin(yRotRad)) * (controls.Get(38) ? MOUVEMENT_SPEED_RUN * elapsedTime : distance);
		m_pos.z -= float(cos(yRotRad)) * (controls.Get(38) ? MOUVEMENT_SPEED_RUN * elapsedTime: distance);
		if (ghost)
			m_pos.y -= float(sin(xRotRad)) * (controls.Get(38) ? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (elapsedTime);
	}
	else
	{
		m_speed.z = 0;
	}

	if (controls.Get(18))	// S
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT2, Son::CHANNEL_STEP, false);
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
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
		float yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		m_pos.x += float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_pos.z += float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}

	if (controls.Get(0))	// A
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
		float yRotRad;
		yRotRad = (m_rot.y / 180 * PII);
		m_pos.x -= float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_pos.z -= float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}
	if (controls.Get(57))	// Space
	{
		if (ghost)
		{
			m_pos.y += MOUVEMENT_SPEED * (1.f + elapsedTime);
		}
		else
		{
			if (m_speed.y == 0)
			{
				m_speed.y = MOUVEMENT_ACCELERATION_UP;
				Info::Get().Sound().PlaySnd(Son::SON_JUMP, Son::CHANNEL_PLAYER, false);
			}
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
