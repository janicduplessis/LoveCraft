#include "player.h"
#include "openglcontext.h"
#include "son.h"
#include "info.h"
#include <cmath>
#include <iostream>
#include <SFML/Network.hpp>

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
	m_model.Scale(0.5f, 0.9f, 0.5f);
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
		float distance = (m_speed.y * elapsedTime) + (GRAVITY * elapsedTime * elapsedTime / 2.0f);
		std::cout << distance << std::endl;
		if (!CheckCollision(Vector3f(m_pos.x, m_pos.y - distance, m_pos.z)))
		{
			m_pos.y -= distance;
			m_speed.y = m_speed.y + (GRAVITY * elapsedTime);
		}
		else
		{
			if (m_speed.y != 0)
				Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
			m_speed.y = 0;
		}
	}
	if (controls.Get(22))	// W -z
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);

		// Détermine la vitesse max et l'acceleration en fontion de si run est activé
		float xRotRad = (m_rot.x / 180 * PII);
		float yRotRad = (m_rot.y / 180 * PII);
		float speedMaxX = sin(yRotRad) * controls.Get(38) ? MOUVEMENT_SPEED_MAX_RUN : MOUVEMENT_SPEED_MAX;
		float speedMaxY = cos(yRotRad) * controls.Get(38) ? MOUVEMENT_SPEED_MAX_RUN : MOUVEMENT_SPEED_MAX;
		float accel = controls.Get(38) ? MOUVEMENT_ACCELERATION_RUN : MOUVEMENT_ACCELERATION;

		m_speed.x += sin(yRotRad) * m_accel.x * elapsedTime;
		m_speed.z -= cos(yRotRad) * m_accel.z * elapsedTime;

		// Applique la vitesse initiale
		//if (m_speed.z == 0)
		//	m_speed.z += MOUVEMENT_SPEED_INI;

		// Regarde si la vitesse dépasse la vitesse max
		if (m_speed.x <= speedMaxX) {
			m_accel.x = 0;
			m_speed.x = speedMaxX;
		} else {
			m_accel.x = accel;
		}

		// Calcul la nouvelle position et la nouvelle vitesse
		Vector3f newPos;
		float distance = (m_speed.z * elapsedTime) + (m_accel.z * elapsedTime * elapsedTime / 2.0f);

		newPos.x = m_pos.x + float(sin(yRotRad)) * distance;
		newPos.z = m_pos.z - float(cos(yRotRad)) * distance;
		newPos.y = m_pos.y - ((ghost) ? float(sin(xRotRad)) * distance : 0);

		// Si pas de collision affecter la nouvelle position et vitesse
		if (!CheckCollision(newPos))
		{
			m_speed.z = m_speed.z + (m_accel.z * elapsedTime);
			m_pos = newPos;
		}
	}
	else
	{
		m_speed.z = 0;
	}

	if (controls.Get(18))	// S +z
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT2, Son::CHANNEL_STEP, false);

		// Détermine la vitesse max et l'acceleration en fontion de si run est activé
		float speedMax = controls.Get(38) ? MOUVEMENT_SPEED_MAX_RUN : MOUVEMENT_SPEED_MAX;
		float accel = controls.Get(38) ? MOUVEMENT_ACCELERATION_RUN : MOUVEMENT_ACCELERATION;

		// Applique la vitesse initiale
		if (m_speed.z == 0)
			m_speed.z += MOUVEMENT_SPEED_INI;

		// Regarde si la vitesse dépasse la vitesse max
		if (m_speed.z >= speedMax) {
			m_speed.z = speedMax;
			m_accel.z = 0;
		}
		else m_accel.z = accel;

		// Calcul la nouvelle position et la nouvelle vitesse
		Vector3f newPos;
		float distance = (m_speed.z * elapsedTime) + (m_accel.z * elapsedTime * elapsedTime / 2.0f);
		float xRotRad = (m_rot.x / 180 * PII);
		float yRotRad = (m_rot.y / 180 * PII);

		newPos.x = m_pos.x - float(sin(yRotRad)) * distance;
		newPos.z = m_pos.z + float(cos(yRotRad)) * distance;
		newPos.y = m_pos.y + ((ghost) ? float(sin(xRotRad)) * distance : 0);

		// Si pas de collision affecter la nouvelle position et vitesse
		if (!CheckCollision(newPos))
		{
			m_speed.z = m_speed.z + (m_accel.z * elapsedTime);
			m_pos = newPos;
		}
	}
	if (controls.Get(3))	// D +x
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);

		// Détermine la vitesse max et l'acceleration en fontion de si run est activé
		float speedMax = controls.Get(38) ? MOUVEMENT_SPEED_MAX_RUN : MOUVEMENT_SPEED_MAX;
		float accel = controls.Get(38) ? MOUVEMENT_ACCELERATION_RUN : MOUVEMENT_ACCELERATION;

		// Applique la vitesse initiale
		if (m_speed.x == 0)
			m_speed.x += MOUVEMENT_SPEED_INI;

		// Regarde si la vitesse dépasse la vitesse max
		if (m_speed.x >= speedMax) {
			m_speed.x = speedMax;
			m_accel.x = 0;
		}
		else m_accel.x = accel;

		// Calcul la nouvelle position et la nouvelle vitesse
		Vector3f newPos;
		float distance = (m_speed.x * elapsedTime) + (m_accel.x * elapsedTime * elapsedTime / 2.0f);
		float yRotRad = (m_rot.y / 180 * PII);

		newPos.x = m_pos.x + float(cos(yRotRad)) * distance;
		newPos.z = m_pos.z + float(sin(yRotRad)) * distance;
		newPos.y = m_pos.y;

		// Si pas de collision affecter la nouvelle position et vitesse
		if (!CheckCollision(newPos))
		{
			m_speed.x = m_speed.x + (m_accel.x * elapsedTime);
			m_pos = newPos;
		}
	}

	if (controls.Get(0))	// A -x
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);

		// Détermine la vitesse max et l'acceleration en fontion de si run est activé
		float speedMax = controls.Get(38) ? MOUVEMENT_SPEED_MAX_RUN : MOUVEMENT_SPEED_MAX;
		float accel = controls.Get(38) ? MOUVEMENT_ACCELERATION_RUN : MOUVEMENT_ACCELERATION;

		// Applique la vitesse initiale
		if (m_speed.x == 0)
			m_speed.x += MOUVEMENT_SPEED_INI;

		// Regarde si la vitesse dépasse la vitesse max
		if (m_speed.x >= speedMax) {
			m_speed.x = speedMax;
			m_accel.x = 0;
		}
		else m_accel.x = accel;

		// Calcul la nouvelle position et la nouvelle vitesse
		Vector3f newPos;
		float distance = (m_speed.x * elapsedTime) + (m_accel.x * elapsedTime * elapsedTime / 2.0f);
		float yRotRad = (m_rot.y / 180 * PII);

		newPos.x = m_pos.x - float(cos(yRotRad)) * distance;
		newPos.z = m_pos.z - float(sin(yRotRad)) * distance;
		newPos.y = m_pos.y;

		// Si pas de collision affecter la nouvelle position et vitesse
		if (!CheckCollision(newPos))
		{
			m_speed.x = m_speed.x + (m_accel.x * elapsedTime);
			m_pos = newPos;
		}
	}
	if (controls.Get(57))	// Space
	{
		if (!ghost)
		{
			if (m_speed.y == 0)
			{
				m_speed.y = -MOUVEMENT_SPEED_JUMP;
				Info::Get().Sound().PlaySnd(Son::SON_JUMP, Son::CHANNEL_PLAYER, false);
			}
		}
		else m_pos.y += MOUVEMENT_SPEED_MAX * elapsedTime;
	}
	if (controls.Get(37))	// Ctrl
	{
		if (!ghost)
		{
			//Code du crouch ici
		}
		else m_pos.y -= MOUVEMENT_SPEED_MAX * elapsedTime;
	}
	//Test réseau - Envoie d'un paquet contenant les coordonnées du joueur sur le réseau
	//sf::Packet pack;
	//pack << m_pos.x << m_pos.y << m_pos.z;
	//Info::Get().Network().Send(pack);
}

void Player::Render(bool wireFrame)
{
	m_model.SetPosition(Vector3f(m_pos.x, m_pos.y - 0.5, m_pos.z));
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

bool Player::CheckCollision(const Vector3f& pos) const
{
	static float offset = 0.2f;

	if(pos.y >=0 
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(offset, 1, offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(-offset, 1, offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(-offset, 1, -offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(offset, 1, -offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(offset, 2.8f, offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(-offset, 2.8f, offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(-offset, 2.8f, -offset)) == BTYPE_AIR
		&& Info::Get().GetBlocFromWorld(pos, Vector3f(offset, 2.8f, -offset)) == BTYPE_AIR)
		return false;
	else 
		return true;
}
