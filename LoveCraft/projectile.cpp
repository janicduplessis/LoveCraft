#include "projectile.h"
#include "matrix4.h"
#include <cmath>
#include <iostream>


Projectile::Projectile() : m_speedIni(0), m_hasMass(false), m_timeToLive(99999999), 
	m_destination(0), m_acceleration(0), m_shot(false), m_maxRot(0.01f), m_collisionRadius(0.01f, 0.01f, 0.01f)
{

}

Projectile::~Projectile()
{

}

void Projectile::Move(float elapsedTime) 
{
	// Test si les conditions de fin du projectile sont vraies
	if (m_timeToLive <= 0 || !m_shot)
		return;

	// Test si atteint la cible
	if(	abs(m_destination.x - m_pos.x) < m_collisionRadius.x 
		&& abs(m_destination.y - m_pos.y) < m_collisionRadius.y 
		&& abs(m_destination.z - m_pos.z) < m_collisionRadius.z) {
			Hit();
			return;
	}

	// Met a jour la valeur de la vitesse en 
	// fonction de l'acceleration et du temps
	m_speed += m_acceleration * elapsedTime;

	m_timeToLive -= elapsedTime;

	// distance entre le projectile et sa destination
	// chemin le plus court
	Vector3f distance;
	distance.x = m_pos.x - m_destination.x;
	distance.y = m_pos.y - m_destination.y;
	distance.z = m_pos.z - m_destination.z;

	// calculer l'angle entre les 2 vecteurs
	// fix imprecision float
	float n = distance.Dot(m_speed) / (distance.Lenght() * m_speed.Lenght());
	if (n > 1)
		n = 1;
	else if (n < -1)
		n = -1;
	float angleA = acos(n);
	Vector3f axis = distance.Cross(m_speed);
	axis.Normalise();
	// rotation autour de laxe
	float rotation;
	if (abs(angleA) >= m_maxRot)
		rotation = (angleA > 0) ? -m_maxRot : m_maxRot;
	else
		rotation = angleA;
	Quaternion q;
	q.FromAxis(rotation, axis);
	q.Normalise();

	float speed = m_speed.Lenght();
	m_speed = q * m_speed * speed;

	// calcul la nouvelle position
	m_pos += m_speed * elapsedTime;
}

void Projectile::Shoot() 
{
	m_shot = true;
	// Modifie l'acceleration pour inclure la
	// gravité s'il a lieux
	if (m_hasMass)
		m_acceleration.y -= 9.8f;

	m_speed = m_speedIni;
}


void Projectile::Hit()
{

}


void Projectile::SetInitialSpeed( const Vector3f& iniSpeed )
{
	m_speedIni = iniSpeed;
}

Vector3f Projectile::GetInitialSpeed() const
{
	return m_speedIni;
}

void Projectile::SetAcceleration( const Vector3f& acc )
{
	m_acceleration = acc;
}

Vector3f Projectile::GetAcceleration() const
{
	return m_acceleration;
}

Vector3f Projectile::GetSpeed() const
{
	return m_speed;
}

void Projectile::SetPosition( const Vector3f& pos )
{
	m_pos = pos;
}

Vector3f Projectile::GetPosition() const
{
	return m_pos;
}

void Projectile::SetDestination( const Vector3f& dest )
{
	m_destination = dest;
}

Vector3f Projectile::GetDestination() const
{
	return m_destination;
}

void Projectile::SetTimeToLive( float time )
{
	m_timeToLive = time;
}

float Projectile::GetTimeToLive() const
{
	return m_timeToLive;
}

void Projectile::SetHasMass( bool hasMass )
{
	m_hasMass = hasMass;
}

bool Projectile::HasMass() const
{
	return m_hasMass;
}

void Projectile::SetMaxRot( float maxRot )
{
	m_maxRot = maxRot;
}

void Projectile::SetCollisionRadius(const Vector3f& rad )
{
	m_collisionRadius = rad;
}

