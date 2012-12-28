#include "ai.h"

#include <cmath>

AI::AI(AiType type, Npc* npc, Player* player) : m_player(player), m_type(type), m_npc(npc)
{

}

AI::~AI()
{

}


bool AI::StateChanged() const
{
	return m_stateChanged;
}

void AI::Process()
{
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
	if (m_currentState != STATE_PATROL)
		m_stateChanged = true;


}

void AI::RunAway()
{
	if (m_currentState != STATE_RUN_AWAY)
		m_stateChanged = true;
}

void AI::Attack()
{
	if (m_currentState != STATE_ATTACK)
		m_stateChanged = true;
}

void AI::Follow()
{
	if (m_currentState != STATE_FOLLOW)
		m_stateChanged = true;
}

void AI::Stay()
{
	if (m_currentState != STATE_STAY)
		m_stateChanged = true;
}
