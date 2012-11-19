#include "Player.h"
#include "openglcontext.h"
#include <cmath>
#include <iostream>


Player::Player() :m_posX(0.f), m_posY(0.f), m_posZ(0.f), m_rotX(0.f), m_rotY(0.f), m_lastX(0), m_lastY(0)
{
}

Player::Player(float posX, float posY, float posZ, float rotX, float rotY)
	:m_posX(posX), m_posY(posY), m_posZ(posZ), m_rotX(rotX), m_rotY(rotY), m_lastX(0), m_lastY(0)
{
}

Player::~Player()
{
}

void Player::TurnLeftRight ( float value )
{
	//std::cout << "Valeur de RotY: " << m_rotY << std::endl;	//Debug de l'angle
	m_rotY += value * MOUSE_SENSIBILITY;
}

void Player::TurnTopBottom ( float value )
{
	//std::cout << "Valeur de RotX: " << m_rotX << std::endl;	//Debug de l'angle

	//Assignation de la nouvelle rotation
	float newRotation = m_rotX + value * MOUSE_SENSIBILITY;
	//Test de la rotation entre les limites établies
	if (newRotation >= -85.f && newRotation <= 85.f)
		m_rotX = newRotation;
}

void Player::Move ( bool front , bool back , bool left , bool right , bool run, bool ghost, float elapsedTime )
{
	if (front)
	{
		float xrotrad, yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		xrotrad = (m_rotX / 180 * PII);
		m_posX += float(sin(yrotrad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		m_posZ -= float(cos(yrotrad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		if (ghost)
			m_posY -= float(sin(xrotrad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
	}

	if (back)
	{
		float xrotrad, yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		xrotrad = (m_rotX / 180 * PII);
		m_posX -= float(sin(yrotrad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		m_posZ += float(cos(yrotrad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		if (ghost)
			m_posY += float(sin(xrotrad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
	}
	if (right)
	{
		float yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		m_posX += float(cos(yrotrad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_posZ += float(sin(yrotrad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}

	if (left)
	{
		float yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		m_posX -= float(cos(yrotrad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_posZ -= float(sin(yrotrad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}
}

void Player::ApplyRotation () const
{
	glRotatef(m_rotX,1.f,0.f,0.f);
	glRotatef(m_rotY,0.f,1.f,0.f);
}
void Player::ApplyTranslation () const
{
	glTranslatef(-m_posX, -m_posY, -m_posZ);
}