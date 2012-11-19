#include "Player.h"
#include "openglcontext.h"
#include <cmath>
#include <iostream>

Player::Player(float posX, float posY, float posZ, float rotX, float rotY)
	:m_posX(posX), m_posY(posY), m_posZ(posZ), m_rotX(rotX), m_rotY(rotY)
{
}

Player::~Player()
{
}

void Player::TurnLeftRight ( float value )
{
	m_rotY += value * MOUSE_SENSIBILITY;
}

void Player::TurnTopBottom ( float value )
{
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
		float xRotRad, yRotRad;
		yRotRad = (m_rotY / 180 * PII);
		xRotRad = (m_rotX / 180 * PII);
		m_posX += float(sin(yRotRad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		m_posZ -= float(cos(yRotRad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
		if (ghost)
			m_posY -= float(sin(xRotRad)) * (run? MOUVEMENT_SPEED_RUN : MOUVEMENT_SPEED) * (1.f + elapsedTime);
	}

	if (back)
	{
		float xRotRad, yRotRad;
		yRotRad = (m_rotY / 180 * PII);
		xRotRad = (m_rotX / 180 * PII);
		m_posX -= float(sin(yRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		m_posZ += float(cos(yRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
		if (ghost)
			m_posY += float(sin(xRotRad)) * MOUVEMENT_SPEED_BACKWARD * (1.f + elapsedTime);
	}
	if (right)
	{
		float yRotRad;
		yRotRad = (m_rotY / 180 * PII);
		m_posX += float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_posZ += float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}

	if (left)
	{
		float yRotRad;
		yRotRad = (m_rotY / 180 * PII);
		m_posX -= float(cos(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
		m_posZ -= float(sin(yRotRad)) * MOUVEMENT_SPEED * (1.f + elapsedTime);
	}
}

void Player::ApplyRotation () const
{
	glRotatef(m_rotX, 1.f, 0.f, 0.f);
	glRotatef(m_rotY, 0.f, 1.f, 0.f);
}
void Player::ApplyTranslation () const
{
	glTranslatef(-m_posX, -m_posY, -m_posZ);
}