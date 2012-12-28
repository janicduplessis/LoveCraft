#include "npc.h"

Npc::Npc(const Vector3f& pos /*= Vector3f(0,0,0)*/ ) : m_ai(0)
{

}

Npc::~Npc()
{

}

void Npc::Init()
{

}

void Npc::Update()
{

}

void Npc::Render() const
{
	m_model.Render();
}

void Npc::Move(const Vector3f& destination, float elapsedTime) 
{
	// Test si atteint la cible
	if(	abs(destination.x - m_pos.x) < 1 
		&& abs(destination.y - m_pos.y) < 1 
		&& abs(destination.z - m_pos.z) < 1) {
			return;
	}

	// Met a jour la valeur de la vitesse en 
	// fonction de l'acceleration et du temps
	m_speed += m_acceleration * elapsedTime;

	// distance entre le projectile et sa destination
	// chemin le plus court
	Vector3f distance;
	distance.x = m_pos.x - destination.x;
	distance.y = m_pos.y - destination.y;
	distance.z = m_pos.z - destination.z;

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

Vector3f Npc::Position() const
{
	return m_pos;
}
