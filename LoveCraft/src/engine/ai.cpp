#include "ai.h"

#include <cmath>

AI::AI(AiType type, Npc* npc, Player* player) : 
	m_player(player), m_type(type), m_npc(npc), m_posIni(0), m_currentState(STATE_PATROL),
	m_patrolDestination(0)
{
	m_posIni = m_npc->Position();
}

AI::~AI()
{

}


bool AI::StateChanged() const
{
	return m_stateChanged;
}

void AI::Process(float elapsedTime)
{
	if ((m_player->Position() - m_npc->Position()).Lenght() > VIEW_DISTANCE)
		return;
	// Logique pour determiner le state
	//bool checkPlayer = CheckPlayer(DETECTION_RANGE);
	bool checkPlayer = false;
	// Run Away Start
	if (checkPlayer && m_currentState != STATE_FOLLOW) {
		m_currentState = STATE_FOLLOW;
		m_stateChanged = true;
	}	
	// Run Away End
	if (!checkPlayer) {
		m_currentState = STATE_PATROL;
	}	

	// State par defaut (patrol)
	if (m_currentState == STATE_NONE) {
		m_currentState = STATE_PATROL;
	}

	static float totalTime = 0;
	totalTime += elapsedTime;
	// Call la fonction qui correspond au state
	if (m_currentState == STATE_PATROL)
	{
		if (totalTime > 1 || m_stateChanged)
		{
			totalTime = 0;
			Patrol();
		}
		m_npc->Move(Vector3f(m_patrolDestination->x, 
			(m_type == TYPE_PASSIVE_FLYING) ? m_patrolDestination->y : m_npc->Position().y, 
			m_patrolDestination->z), elapsedTime);
	} 
	else if (m_currentState == STATE_FOLLOW)
	{
		m_npc->Move(Vector3f(m_player->Position().x, m_npc->Position().y, m_player->Position().z), elapsedTime);
	}
	else if (m_currentState == STATE_RUN_AWAY)
	{
		// Calcul position opposé au joueur
		Vector3f dist = -PlayerDistance();
		dist += m_npc->Position();
		m_npc->Move(Vector3f(dist.x, m_npc->Position().y, dist.z), elapsedTime);
	}
	m_stateChanged = false;
}

bool AI::CheckVision(const Vector3f& pos) const
{
	Info& info = Info::Get();
	Vector3f vision = pos;
	if (vision.y < 0)
		vision.y = 0;
	vision.y += 1;
	if(info.GetBlocFromWorld(vision) == BTYPE_AIR
		&& info.GetBlocFromWorld(vision) == BTYPE_AIR
		&& info.GetBlocFromWorld(vision) == BTYPE_AIR
		&& info.GetBlocFromWorld(vision) == BTYPE_AIR)
		return true; 
	return false;
}

bool AI::CheckCollision(Vector3f& pos ) const
{
	float offset = 0.3f;
	Info& info = Info::Get();
	if (pos.y < 0)
		pos.y = 0;
	if(!info.GetBlocInfo(info.GetBlocFromWorld(pos, Vector3f(offset, 1, offset)))->IsSolid()
		&& !info.GetBlocInfo(info.GetBlocFromWorld(pos, Vector3f(-offset, 1, offset)))->IsSolid()
		&& !info.GetBlocInfo(info.GetBlocFromWorld(pos, Vector3f(-offset, 1, -offset)))->IsSolid()
		&& !info.GetBlocInfo(info.GetBlocFromWorld(pos, Vector3f(offset, 1, -offset)))->IsSolid())
		return false;
	return true;
}

bool AI::CheckPlayer(float detectionRange, bool enableLOS)
{
	Vector3f distance = PlayerDistance();
	// regarde si le joueur est assez proche
	if (distance.Lenght() > detectionRange)
		return false;

	if (enableLOS) {
		Vector3f npcPos = m_npc->Position();
		Vector3f iterator = distance;
		iterator = iterator.Normalize() / 5;

		// itère sur le vecteur distance et regarde si il y a
		// collision a chaque 0.2m
		for (float i = 0; i < distance.Lenght() * 5; i++)
		{
			if(!CheckVision(npcPos + (iterator * i)))
				return false;
		}
	}
	// passe tous les test, le joueur est visible!
	return true;
}

Vector3f AI::PlayerDistance()
{
	return m_player->Position() - m_npc->Position();
}

bool AI::CheckAttack()
{
	//NYI
	return false;
}

AI::State AI::GetState() const
{
	return m_currentState;
}

void AI::Patrol()
{
	if(CheckCollision(m_npc->Position() + Vector3f(0,1,0)) && !CheckCollision(m_npc->Position() + Vector3f(0,2,0))) {
		m_npc->SetPosition(m_npc->Position() + Vector3f(0,1,0));
	}

	if (!m_patrolDestination || rand() % 100 < 10)
	{
		if (m_patrolDestination)
			delete m_patrolDestination;
		float x = m_posIni.x + rand() % (2 * PATROL_RANGE) - PATROL_RANGE;
		float y = m_posIni.y + rand() % (2 * FLYING_Y_RANGE) - FLYING_Y_RANGE;
		float z = m_posIni.z + rand() % (2 * PATROL_RANGE) - PATROL_RANGE; 
		m_patrolDestination = new Vector3f(x, y, z);
	}
}

void AI::RunAway()
{

}

void AI::Attack()
{

}

void AI::Follow()
{

}

void AI::Stay()
{

}

void AI::SetInitialPosition(Vector3f pos)
{
	m_posIni = pos;
}
