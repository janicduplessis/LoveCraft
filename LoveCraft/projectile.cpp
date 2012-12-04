#include "projectile.h"
#include <cmath>


Projectile::Projectile() : m_speedIni(0), m_hasMass(false), m_timeToLive(999999999999), 
	m_destination(0), m_acceleration(0), m_shot(false)
{

}

Projectile::~Projectile()
{

}

void Projectile::Move(float elapsedTime) 
{
	// Test si les conditions de fin du projectile sont vraies
	if (m_timeToLive <= 0 || m_pos == m_destination || !m_shot)
		return;

	// Met a jour la valeur de la vitesse en 
	// fonction de l'acceleration et du temps
	m_speed += m_acceleration * elapsedTime;

	m_timeToLive -= elapsedTime;

	// distance entre le projectile et sa destination
	// chemin le plus court
	Vector3f distance;
	distance.x = abs(m_pos.x - m_destination.x);
	distance.y = abs(m_pos.y - m_destination.y);
	distance.z = abs(m_pos.z - m_destination.z);

	// si le projectile ne se dirige pas directement vers sa destination
	// (vecteur distance perpendiculaire a vitesse)
	if (m_speed.Cross(distance) != Vector3f())
	{
		// calculer l'angle entre les 2 vecteurs
		float a = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
		float b = sqrt(m_speed.x * m_speed.x + m_speed.y * m_speed.y + m_speed.z * m_speed.z);

		float angleA = std::acosf(distance.Dot(m_speed) / (a * b));

		Vector3f axis = distance.Cross(m_speed);

		// calculer angle a ajouter ou soustraire
		// utiliser sens de l'axe

		Quaternion q;
		Quaternion l;
		l.SetRotation(angleA, axis);

		q = l * q;

		//q.RotationMatrix() * m_speed;
	}
		
	

}

void Projectile::Shoot() 
{
	m_shot = true;
	// Modifie l'acceleration pour inclure la
	// gravité s'il a lieux
	if (m_hasMass)
		m_acceleration.y -= 9.8f;

	m_speed = m_speedIni;

	float x = std::abs(m_pos.x - m_destination.x);
	float y = std::abs(m_pos.z - m_destination.z);

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

