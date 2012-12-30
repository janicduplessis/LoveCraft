#include "npc.h"

Npc::Npc(const Vector3f& pos /*= Vector3f(0,0,0)*/ ) : m_ai(0), m_maxRot(0.01f), m_acceleration(0), m_speed(0)
{

}

Npc::~Npc()
{

}

void Npc::Init(Player* player)
{

}

void Npc::Update(float elapsedTime)
{
	m_ai->Process(elapsedTime);
	m_model.SetRotation(m_rot.GetConjugate());
	m_model.SetPosition(m_pos);
}

void Npc::Render() const
{
	m_model.Render();
}

void Npc::Move(const Vector3f& destination, float elapsedTime) 
{
	if (elapsedTime == 0)
		return;
	// Gravité
	float distanceY = (m_speed.y * elapsedTime) + (m_acceleration.y * elapsedTime * elapsedTime / 2.0f);
	if (!CheckCollision(Vector3f(m_pos.x, m_pos.y - distanceY, m_pos.z)))
	{
		m_acceleration.y = -GRAVITY;
	}
	else
	{
  		m_acceleration.y = 0;
		m_speed.y = 0;
	}

	// Test si atteint la destination
	if(	abs(destination.x - m_pos.x) < 1
		&& abs(destination.y - m_pos.y) < 1 
		&& abs(destination.z - m_pos.z) < 1) {
			return;
	}
	Vector3f deplacement;
	deplacement = (m_speed * elapsedTime) + (m_acceleration * elapsedTime * elapsedTime / 2);
	Vector3f depNoY = Vector3f(deplacement.x, 0, deplacement.z);
	deplacement = m_rot * deplacement;
	depNoY = m_rot * depNoY;
	// Met a jour la valeur de la vitesse en 
	// fonction de l'acceleration et du temps
	m_speed = m_speed + (m_acceleration * elapsedTime);

	// distance entre le projectile et sa destination
	// chemin le plus court
	Vector3f distance;
	distance.x = m_pos.x - destination.x;
	distance.y = m_pos.y - destination.y;
	distance.z = m_pos.z - destination.z;

	// calculer l'angle entre les 2 vecteurs
	// fix imprecision float
	float n = distance.Dot(depNoY) / (distance.Lenght() * depNoY.Lenght());
	if (n > 1)
		n = 1;
	else if (n < -1)
		n = -1;
	float angleA = acos(n);
	Vector3f axis = distance.Cross(depNoY);
	axis.Normalise();
	// rotation autour de laxe
	float rotation;
	if (abs(angleA) >= m_maxRot && abs(angleA) < PII - m_maxRot)
		rotation = (angleA > 0) ? -m_maxRot : m_maxRot;
	else
		rotation = angleA - PII;
	Quaternion q;
	q.FromAxis(rotation, axis);
	q.Normalise();

	m_rot = q * m_rot;
	m_rot.Normalise();

	// calcul la nouvelle position
	m_pos += deplacement;
	m_pos.Afficher();
}

bool Npc::CheckCollision(const Vector3f& pos) const
{
	static float offset = 0.2f;
	Info& info = Info::Get();
	if(pos.y >=0 
		&& info.GetBlocFromWorld(pos, Vector3f(offset, 1, offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(-offset, 1, offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(-offset, 1, -offset)) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos, Vector3f(offset, 1, -offset)) == BTYPE_AIR)
		return false; 
	return true;
}

Vector3f Npc::Position() const
{
	return m_pos;
}

void Npc::SetPosition(const Vector3f& pos)
{
	m_pos = pos;
}
