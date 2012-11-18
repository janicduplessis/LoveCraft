#include "Player.h"
#include "openglcontext.h"
#include <cmath>


Player::Player() :m_posX(0.f), m_posY(0.f), m_posZ(0.f), m_rotX(0.f), m_rotY(0.f)
{
}

Player::Player(float posX, float posY, float posZ, float rotX, float rotY)
	:m_posX(posX), m_posY(posY), m_posZ(posZ), m_rotX(rotX), m_rotY(rotY)
{
}

Player::~Player()
{
}


void Player::TurnLeftRight ( float value )
{

}
void Player::TurnTopBottom ( float value )
{

}

void Player::Move ( bool front , bool back , bool left , bool right , float elapsedTime )
{
	if (front)
	{
		float xrotrad, yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		xrotrad = (m_rotX / 180 * PII);
		m_posX += float(sin(yrotrad)) ;
		m_posZ -= float(cos(yrotrad)) ;
		m_posY -= float(sin(xrotrad)) ;
	}

	if (back)
	{
		float xrotrad, yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		xrotrad = (m_rotX / 180 * PII);
		m_posX -= float(sin(yrotrad));
		m_posZ += float(cos(yrotrad)) ;
		m_posY += float(sin(xrotrad));
	}
	if (right)
	{
		float yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		m_posX += float(cos(yrotrad)) * MOUSE_SENSIBILITY;
		m_posZ += float(sin(yrotrad)) * MOUSE_SENSIBILITY;
	}

	if (left)
	{
		float yrotrad;
		yrotrad = (m_rotY / 180 * PII);
		m_posX -= float(cos(yrotrad)) * MOUSE_SENSIBILITY;
		m_posZ -= float(sin(yrotrad)) * MOUSE_SENSIBILITY;
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