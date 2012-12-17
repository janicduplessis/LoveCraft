#include "projectile.h"
#include "matrix4.h"
#include <cmath>
#include <iostream>


Projectile::Projectile() : m_speedIni(0), m_hasMass(false), m_timeToLive(99999999), 
	m_destination(0), m_acceleration(0), m_shot(false)
{

}

Projectile::~Projectile()
{

}

void Projectile::TestRotation()
{
	if (!m_shot)
		return;
	Quaternion l;
	l.SetRotation(0.01, Vector3f(1, 0, 0));
	
	m_rotation = l * m_rotation;
	m_rotation.Normalize();
	//m_rotation.Afficher();

	Matrix4f rot = m_rotation.RotationMatrix();
	m_speed = Vector3f(1,0,0);
	m_speed.x = rot.Get11() * m_speed.x + rot.Get12() * m_speed.y + rot.Get13() * m_speed.z;
	m_speed.y = rot.Get21() * m_speed.x + rot.Get22() * m_speed.y + rot.Get23() * m_speed.z;
	m_speed.z = rot.Get31() * m_speed.x + rot.Get32() * m_speed.y + rot.Get33() * m_speed.z;
	m_pos = m_pos + m_speed;
	/*std::cout << rot.Get11() << std::endl;
	std::cout << rot.Get21() << std::endl;
	std::cout << rot.Get31() << std::endl;
	std::cout << rot.Get41() << std::endl;*/
	m_speed.Afficher();
	m_rotation.Afficher();
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
	if (m_speed.Cross(distance) != Vector3f(0,0,0))
	{
		// calculer l'angle entre les 2 vecteurs
		float angleA = std::acosf(distance.Dot(m_speed) / (distance.Lenght() * m_speed.Lenght()));
		Vector3f axis = distance.Cross(m_speed);
		axis.Normalize();
		// rotation autour de laxe
		Quaternion q;
		Quaternion l;
		//l.SetRotation((angleA * elapsedTime < 0.02f) ? angleA * elapsedTime : 0.02f, axis);
		l.SetRotation(-0.02f, axis);
		q = l * q;
		Matrix4f& rot = q.RotationMatrix();

		// matrice de rotation * speed
		m_speed.x = rot.Get11() * m_speed.x + rot.Get12() * m_speed.y + rot.Get13() * m_speed.z;
		m_speed.y = rot.Get21() * m_speed.x + rot.Get22() * m_speed.y + rot.Get23() * m_speed.z;
		m_speed.z = rot.Get31() * m_speed.x + rot.Get32() * m_speed.y + rot.Get33() * m_speed.z;
	}

	m_speed.Afficher();
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

	//m_speed = m_speedIni;
	m_speed = Vector3f(1, 0, 0);
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

