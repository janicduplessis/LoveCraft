#include "player.h"
#include "openglcontext.h"
#include "son.h"
#include "info.h"
#include "engine.h"
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

void Player::Move(bool ghost, float &health, float &energy, float elapsedTime )
{

#pragma region Raccourcis des touches

	bool w = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool s = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool a = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool d = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	bool c = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
	bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	bool space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

#pragma endregion

	// =====================================================

#pragma region Mouvements

#pragma region Mouvement en Y

#pragma region Calcul de la position en fonction de la gravite
	if (!ghost)
	{
		m_accel.y = GRAVITY;
		float distance = (m_speed.y * elapsedTime) + (m_accel.y * elapsedTime * elapsedTime / 2.0f);
		if (!CheckCollision(Vector3f(m_pos.x, m_pos.y - distance, m_pos.z)))
		{
			m_pos.y -= distance;
			m_speed.y = m_speed.y + (m_accel.y * elapsedTime);
		}
		else
		{
			if (m_speed.y != 0)
				Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
			//Perte de vie quand on tombe de trop haut
			if (m_speed.y > 8)
				health -= (int)(m_speed.y * HEALTH_GRAVITY_LOST);
			m_speed.y = 0;
		}
	}
#pragma endregion

#pragma region Calcul de la vitesse lors d un saut
	if (space)
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
#pragma endregion

#pragma region Position en fonction du crouch

	if (c)	// Ctrl
	{
		if (!ghost)
		{
			//Code du crouch ici
		}
		else m_pos.y -= MOUVEMENT_SPEED_MAX * elapsedTime;
	}

#pragma endregion

#pragma endregion

#pragma region Mouvement en Z

#pragma region Calculs de la nouvelle position en fonction de la vitesse en Z
	if (m_speed.z != 0)
	{
		Vector3f newPos;
		float distance = (m_speed.z * elapsedTime) + (m_accel.z * elapsedTime * elapsedTime / 2.0f);
		float xRotRad = (m_rot.x / 180 * PII);
		float yRotRad = (m_rot.y / 180 * PII);
		newPos.x = m_pos.x + float(sin(yRotRad)) * distance;
		newPos.z = m_pos.z - float(cos(yRotRad)) * distance;
		newPos.y = m_pos.y - ((ghost) ? float(sin(xRotRad)) * distance : 0);

		// Si pas de collision affecter la nouvelle position
		if (!CheckCollision(newPos))
		{
			m_lastPos = m_pos;
			m_pos = newPos;
			//Joue le bruit de pas s'il y a eu un mouvement et qu'il n'est pas au ralenti
			if (!ghost && m_speed.y == 0 && m_speed.x != 0 && m_speed.z != 0 && !ctrl)
				Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
		}
	}
#pragma endregion

#pragma region Calculs de la friction
	//Le joueur est-il en mouvement?
	if (m_speed.z != 0)
	{
		//Vérification si le joueur bouge par lui-même (touches)
		if (!w && !s)
		{
			//Si ce n'est pas le cas on diminue progressivement la vitesse du joueur
			//Vérification que la vitesse ne dépasse pas 0
			if (fabs(m_speed.z) - MOUVEMENT_SPEED_DECAY < 0)
				m_speed.z = 0;
			else m_speed.z -= m_speed.z > 0 ? MOUVEMENT_SPEED_DECAY : -MOUVEMENT_SPEED_DECAY;

		}
	}
#pragma endregion

#pragma region Calculs de la vitesse maximale
	//Assignation de base de la vitesse maximale
	float speedMaxZ = MOUVEMENT_SPEED_MAX;
	//vérification si le joueur est en mode course
	if (shift && w && energy > 0)
		speedMaxZ *= MOUVEMENT_SPEED_MAX_RUN_M;
	else if (s)
		//Est-ce qu'il recule
		speedMaxZ *= MOUVEMENT_SPEED_BACKWARD_M;
	//Vérification s'il est au ralenti
	if (ctrl)
		speedMaxZ *= MOUVEMENT_SPEED_SLOW_M;
	//Vérification s'il appuie sur plus d'une touche
	if (w && d || w && a || s && a || s && d)
		speedMaxZ *= 0.6f;
#pragma endregion

#pragma region Calculs de l acceleration
	//Tant qu'il est en mouvement et qu'il n'a pas atteint la vitesse maximum il
	//aura une accélération
	m_accel.z = 0;
	//Accélération positive
	if (w)
		m_accel.z = m_speed.z < speedMaxZ ? MOUVEMENT_ACCELERATION : 0;
	//Accélération négative
	else if (s)
		m_accel.z = m_speed.z < speedMaxZ ? -MOUVEMENT_ACCELERATION : 0;
	//Si le joueur est en mode course et au sol
	if (shift && m_speed.y == 0)
		m_accel.z *= MOUVEMENT_ACCELERATION_RUN_M;
	//Diminution de l'accélération lorsque le joueur est en mouvement dans les airs
	if (m_speed.y != 0)
		m_accel.z *= MOVUEMENT_ACCELERATION_AIR_M;
#pragma endregion

#pragma region Calculs de la vitesse
	//Vérification que la nouvelle vitesse ne dépasse pas le maximum imposé
	if (fabs(m_speed.z) + fabs(m_accel.z) * elapsedTime > speedMaxZ)
		m_speed.z = m_speed.z < 0 ? -speedMaxZ : speedMaxZ;
	//Assignation de la nouvelle vitesse en fonction de l'accélération
	else m_speed.z += m_accel.z * elapsedTime;
#pragma endregion

#pragma endregion

#pragma region Mouvement en X

#pragma region Calculs de la nouvelle position en fonction de la vitesse en X
	if (m_speed.x != 0)
	{
		Vector3f newPos;
		float distance = (m_speed.x * elapsedTime) + (m_accel.x * elapsedTime * elapsedTime / 2.0f);
		float yRotRad = (m_rot.y / 180 * PII);

		newPos.x = m_pos.x + float(cos(yRotRad)) * distance;
		newPos.z = m_pos.z + float(sin(yRotRad)) * distance;
		newPos.y = m_pos.y;

		// Si pas de collision affecter la nouvelle position
		if (!CheckCollision(newPos))
		{
			m_lastPos = m_pos;	//pas utiliser encore
			m_pos = newPos;
			//Joue le bruit de pas s'il y a eu un mouvement et qu'il n'est pas au ralenti
			if (!ghost && m_speed.y == 0 && m_speed.x != 0 && m_speed.z != 0 && !ctrl)
				Info::Get().Sound().PlaySnd(Son::SON_FOOT1, Son::CHANNEL_STEP, false);
		}
	}
#pragma endregion

#pragma region Calculs de la friction
	//Le joueur est-il en mouvement?
	if (m_speed.x != 0)
	{
		//Vérification si le joueur bouge par lui-même (touches)
		if (!a && !d)
		{
			//Si ce n'est pas le cas on diminue progressivement la vitesse du joueur
			//Vérification que la vitesse ne tombe pas négative
			if (fabs(m_speed.x) - MOUVEMENT_SPEED_DECAY < 0)
				m_speed.x = 0;
			else m_speed.x -= m_speed.x > 0 ? MOUVEMENT_SPEED_DECAY : -MOUVEMENT_SPEED_DECAY;

		}
	}
#pragma endregion

#pragma region Calculs de la vitesse maximale
	//Assignation de base de la vitesse maximale
	float speedMaxX = MOUVEMENT_SPEED_MAX;
	//vérification si le joueur est en mode ralenti
	if (ctrl)
		speedMaxX *= MOUVEMENT_SPEED_SLOW_M;
	//Vérification s'il appuie sur plus d'une touche
	if (w && d || w && a || s && a || s && d)
		speedMaxX *= 0.6f;
#pragma endregion

#pragma region Calculs de l acceleration
	//Tant qu'il est en mouvement et qu'il n'a pas atteint la vitesse maximum il
	//aura une accélération
	m_accel.x = 0;
	if (d)
		m_accel.x = m_speed.x < speedMaxX ? MOUVEMENT_ACCELERATION : 0;
	if (a)
		m_accel.x = m_speed.x < speedMaxX ? -MOUVEMENT_ACCELERATION : 0;
	//Diminution de l'accélération lorsque le joueur est en mouvement dans les airs
	if (m_speed.y != 0)
		m_accel.x *= MOVUEMENT_ACCELERATION_AIR_M;
#pragma endregion

#pragma region Calculs de la vitesse
	//Vérification que la nouvelle vitesse ne dépasse pas le maximum imposé
	if (fabs(m_speed.x) + fabs(m_accel.x) * elapsedTime > speedMaxX)
		m_speed.x = m_speed.x < 0 ? -speedMaxX : speedMaxX;
	//Assignation de la nouvelle vitesse en fonction de l'accélération
	else m_speed.x += m_accel.x * elapsedTime;
#pragma endregion

#pragma endregion

	if (shift && m_speed.z > 1)
		energy -= ENERGY_SPENDING;
	else energy += ENERGY_REGEN;
	health += HEALTH_PASSIVE_REGEN;
#pragma endregion

	// =====================================================

#pragma region Tests reseau

	//Envoie d'un paquet contenant les coordonnées du joueur sur le réseau
	//sf::Packet pack;
	//pack << m_pos.x << m_pos.y << m_pos.z;
	//Info::Get().Network().Send(pack);

#pragma endregion
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
