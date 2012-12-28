#ifndef AI_H__
#define AI_H__

#include "player.h"
#include "info.h"
#include "game/npc.h"

#include <util/vector3.h>
#include <util/quaternion.h>

#define PLAYER_DETECTION_RANGE 10
#define PATROL_RANGE 15

class Npc;

class AI
{
public:
	enum State
	{
		STATE_RUN_AWAY,
		STATE_PATROL,
		STATE_FOLLOW,
		STATE_ATTACK,
		STATE_STAY
	};

	/**
	 * Types d'ai
	 * Passive Harmless : n'attaque pas et se sauve si attaqué
	 * Passive : n'attaque pas mais attaque si attaqué
	 * Aggressive : attaque
	 */
	enum AiType
	{
		TYPE_PASSIVE_HARMLESS,
		TYPE_PASSIVE,
		TYPE_AGGRESSIVE
	};

	AI(AiType type, Npc* npc, Player* player = 0);
	virtual ~AI();

	virtual bool StateChanged() const;
	virtual State GetState() const;

	virtual void Process();
	virtual void Patrol();
	virtual void RunAway();
	virtual void Attack();
	virtual void Follow();
	virtual void Stay();

protected:
	virtual bool CheckCollision(const Vector3f& pos) const;
	virtual bool CheckPlayer();
	virtual bool CheckAttack();

protected:
	Player* m_player;
	State m_currentState;
	AiType m_type;
	Npc* m_npc;

	bool m_stateChanged;
};

#endif