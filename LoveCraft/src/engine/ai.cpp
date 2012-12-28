#include "ai.h"

#include <cmath>

AI::AI(AiType type, Npc* npc, Player* player) : 
	m_player(player), m_type(type), m_npc(npc), m_posIni(npc->Position()), m_currentState(STATE_NONE),
	m_patrolDestination(0)
{

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
	// Logique pour determiner le state
	if (m_currentState != STATE_PATROL) {
		m_currentState = STATE_PATROL;
		m_stateChanged = true;
	}
	static float totalTime = 0;
	totalTime += elapsedTime;
	// Call la fonction qui correspond au state
	if (m_currentState = STATE_PATROL)
	{
		if (totalTime > 1 || m_stateChanged)
		{
			totalTime = 0;
			Patrol();
		}
		m_npc->Move(*m_patrolDestination, elapsedTime);
	}
	m_stateChanged = false;
}

bool AI::CheckCollision(const Vector3f& pos) const
{
	Info& info = Info::Get();
	if(pos.y >=0 
		&& info.GetBlocFromWorld(pos) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos) == BTYPE_AIR
		&& info.GetBlocFromWorld(pos) == BTYPE_AIR)
		return false; 
	return true;
}

bool AI::CheckPlayer()
{
	Vector3f npcPos = m_npc->Position();
	Vector3f distance = m_player->Position() - npcPos;
	Vector3f iterator = distance;
	iterator.Normalise();

	// regarde si le joueur est assez proche
	if (distance.Lenght() > PLAYER_DETECTION_RANGE)
		return false;

	// itère sur le vecteur distance et regarde si il y a
	// collision a chaque 1m
	for (float i = 0; i < distance.Lenght(); i++)
	{
		if(CheckCollision(npcPos + (iterator * i)))
			return false;
	}
	// passe tous les test, le joueur est visible!
	return true;
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
	if (!m_patrolDestination || rand() % 100 < 10)
	{
		if (m_patrolDestination)
			delete m_patrolDestination;
		float x = m_posIni.x + rand() % (2 * PATROL_RANGE) - PATROL_RANGE; 
		float z = m_posIni.z + rand() % (2 * PATROL_RANGE) - PATROL_RANGE; 
		m_patrolDestination = new Vector3f(x, m_posIni.y, z);
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
