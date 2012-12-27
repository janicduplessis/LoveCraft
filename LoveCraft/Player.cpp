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

void Player::Move(const Controls& controls, bool ghost, float elapsedTime )
{
	bool w = controls.W();
	bool s = controls.S();
	bool a = controls.A();
	bool d = controls.D();
	//Calcul de la position en fonction de la gravité
	if (!ghost)
	{
		float distance = (m_speed.y * elapsedTime) + (GRAVITY * elapsedTime * elapsedTime / 2.0f);
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
	// ==================================================================================================

	// Calcul la nouvelle position en fonction de la vitesse
	if (m_speed.z > 0)
	{
		Vector3f newPos;
		float distance = (m_speed.z * elapsedTime) + (m_accel.z * elapsedTime * elapsedTime / 2.0f);
		float xRotRad = (m_rot.x / 180 * PII);
		float yRotRad = (m_rot.y / 180 * PII);
		newPos.x = m_pos.x + float(sin(yRotRad)) * distance * (s ? -1 : 1);
		newPos.z = m_pos.z - float(cos(yRotRad)) * distance * (s ? -1 : 1);
		newPos.y = m_pos.y - ((ghost) ? float(sin(xRotRad)) * distance * (s ? -1 : 1) : 0);

		// Si pas de collision affecter la nouvelle position
		if (!CheckCollision(newPos))
		{
			m_lastPos = m_pos;	//pas utiliser encore
			m_pos = newPos;
			//Joue le bruit de pas s'il y a eu un mouvement
			if (!ghost && m_speed.y == 0 && m_speed.x != 0 && m_speed.z != 0)
				Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
		}
	}
	//Calculs de la friction
	//Le joueur est-il en mouvement?
	if (m_speed.z > 0)
	{
		//Vérification si le joueur bouge par lui-même (touches)
		if (!w && !s)
		{
			//Si ce n'est pas le cas on diminue progressivement la vitesse du joueur
			//Vérification que la vitesse ne tombe pas négative
			if (m_speed.z - MOUVEMENT_SPEED_DECAY < 0)
				m_speed.z = 0;
			else m_speed.z -= MOUVEMENT_SPEED_DECAY;

		}
	}

	//Calculs de la vitesse maximale
	//Assignation de base de la vitesse maximale
	float speedMax = MOUVEMENT_SPEED_MAX;
	//vérification si le joueur est en mode course
	if (controls.Shift() && w)
		speedMax *= MOUVEMENT_SPEED_MAX_RUN_M;
	else if (s)
		//Est-ce qu'il recule
		speedMax *= MOUVEMENT_SPEED_BACKWARD_M;

	//Calculs de l'accélération
	//Tant qu'il est en mouvement et qu'il n'a pas atteint la vitesse maximum il
	//aura une accélération
	m_accel.z = ((w || s) && m_speed.z < speedMax) ? MOUVEMENT_ACCELERATION : 0;
	//Si le joueur est en mode course et au sol
	if (controls.Shift() && m_speed.y == 0)
		m_accel.z *= MOUVEMENT_ACCELERATION_RUN_M;
	//Diminution de l'accélération lorsque le joueur est en mouvement dans les airs
	if (m_speed.y != 0)
		m_accel.z *= MOVUEMENT_ACCELERATION_AIR_M;

	//Calculs de la vitesse
	//Vérification que la nouvelle vitesse ne dépasse pas le maximum imposé
	if (m_speed.z + m_accel.z * elapsedTime > speedMax)
		m_speed.z = speedMax;
	//Assignation de la nouvelle vitesse en fonction de l'accélération
	else m_speed.z += m_accel.z * elapsedTime;



	// Détermine la vitesse max et l'acceleration en fontion de si run est activé

	//float accelz = 0;
	//if (w)
	//	accelz = MOUVEMENT_ACCELERATION;



	// ==================================================================================================
	//if (controls.W())	// W -z
	//{
	//	if (!ghost && m_speed.y == 0)
	//		Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);

	//	// Détermine la vitesse max et l'acceleration en fontion de si run est activé
	//	float speedMax = controls.Shift() ? MOUVEMENT_SPEED_MAX_RUN : MOUVEMENT_SPEED_MAX;
	//	float accel = controls.Shift() ? MOUVEMENT_ACCELERATION_RUN : MOUVEMENT_ACCELERATION;

	//	//Applique la vitesse initiale
	//	if (m_speed.z == 0)
	//	{
	//		m_speed.z += MOUVEMENT_SPEED_INI;
	//		m_accel.z = MOUVEMENT_ACCELERATION;
	//	}

	//	// Regarde si la vitesse dépasse la vitesse max
	//	if (m_speed.z >= speedMax) 
	//	{
	//		m_accel.z = 0;
	//		m_speed.z = speedMax;
	//	} 
	//	else 
	//	{
	//		m_accel.z = accel;
	//	}

	//	// Calcul la nouvelle position et la nouvelle vitesse
	//	Vector3f newPos;
	//	float distance = (m_speed.z * elapsedTime) + (m_accel.z * elapsedTime * elapsedTime / 2.0f);
	//	float xRotRad = (m_rot.x / 180 * PII);
	//	float yRotRad = (m_rot.y / 180 * PII);
	//	newPos.x = m_pos.x + float(sin(yRotRad)) * distance;
	//	newPos.z = m_pos.z - float(cos(yRotRad)) * distance;
	//	newPos.y = m_pos.y - ((ghost) ? float(sin(xRotRad)) * distance : 0);

	//	// Si pas de collision affecter la nouvelle position et vitesse
	//	if (!CheckCollision(newPos))
	//	{
	//		m_speed.z = m_speed.z + (m_accel.z * elapsedTime);
	//		m_lastPos = m_pos;
	//		m_pos = newPos;
	//	}
	//}
	//else
	//{
	//	// note Alex ~ Au lieu de mettre le speed à zéro immédiatement
	//	//			   lorsque le joueur arrête, on pourrait décrémenter la vitesse
	//	//			   progressivement jusqu'à atteindre 0

	//	//JD
	//	//m_speed.z = 0;

	//	//AB
	//	if (m_speed.z > 0)
	//	{
	//		m_speed.z -= 0.5f;
	//		if (m_speed.z - 0.5f < 0)
	//			m_speed.z = 0;
	//	}

	//}

	//if (controls.S())	// S +z
	//{
	//	if (!ghost && m_speed.y == 0)
	//		Info::Get().Sound().PlaySnd(Son::SON_FOOT2, Son::CHANNEL_STEP, false);

	//	// Détermine la vitesse max et l'acceleration en fontion de si run est activé
	//	float speedMax = MOUVEMENT_SPEED_MAX;
	//	float accel = MOUVEMENT_ACCELERATION;

	//	// Applique la vitesse initiale
	//	if (m_speed.z == 0)
	//		m_speed.z += MOUVEMENT_SPEED_INI;

	//	// Regarde si la vitesse dépasse la vitesse max
	//	if (m_speed.z >= speedMax) {
	//		m_speed.z = speedMax;
	//		m_accel.z = 0;
	//	}
	//	else m_accel.z = accel;

	//	// Calcul la nouvelle position et la nouvelle vitesse
	//	Vector3f newPos;
	//	float distance = (m_speed.z * elapsedTime) + (m_accel.z * elapsedTime * elapsedTime / 2.0f);
	//	float xRotRad = (m_rot.x / 180 * PII);
	//	float yRotRad = (m_rot.y / 180 * PII);

	//	newPos.x = m_pos.x - float(sin(yRotRad)) * distance;
	//	newPos.z = m_pos.z + float(cos(yRotRad)) * distance;
	//	newPos.y = m_pos.y + ((ghost) ? float(sin(xRotRad)) * distance : 0);

	//	// Si pas de collision affecter la nouvelle position et vitesse
	//	if (!CheckCollision(newPos))
	//	{
	//		m_speed.z = m_speed.z + (m_accel.z * elapsedTime);
	//		m_pos = newPos;
	//	}
	//}
	if (controls.D())	// D +x
	{
		if (!ghost && m_speed.y == 0)
			Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);

		// Détermine la vitesse max et l'acceleration en fontion de si run est activé
		float speedMax = MOUVEMENT_SPEED_MAX;
		float accel = MOUVEMENT_ACCELERATION;

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
		float speedMax = MOUVEMENT_SPEED_MAX;
		float accel = MOUVEMENT_ACCELERATION;

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

Vector3f Player::Speed() const
{
	return m_speed;
}

Vector3f Player::Acceleration() const
{
	return m_accel;
}

void Player::SetRotation( Vector2f rot )
{
	m_rot = rot;
}

bool Player::CheckCollision(const Vector3f& pos) const
{
	static float offset = 0.2f;
	Info& info = Info::Get();
	if(pos.y >=0 
		&& info.GetBlocFromWorld(pos, Vector3f(offset, 1, offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(-offset, 1, offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(-offset, 1, -offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(offset, 1, -offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(offset, 2.8f, offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(-offset, 2.8f, offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(-offset, 2.8f, -offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(offset, 2.8f, -offset)) == BTYPE_AIR)
		return false; 
	return true;
}
