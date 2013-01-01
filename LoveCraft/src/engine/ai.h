#ifndef AI_H__
#define AI_H__

#include "player.h"
#include "info.h"
#include <game/npc.h>

#include <util/vector3.h>

#define DETECTION_RANGE 10
#define RUN_AWAY_RANGE 20
#define PATROL_RANGE 30

class Npc;
/**
 * @brief Intelligence Artificielle
 */
class AI
{
public:
	/**
	 * @brief �tats de l'AI
	 * 
	 * Run Away :	se sauve du joueur
	 * Patrol :		se prom�ne al�atoirement dans son territoire
	 * Follow :		suit le joueur
	 * Attack :		attaque le joueur
	 * Stay :		reste sur place
	 * None :		aucun, state par defaut est utilis�
	 */
	enum State
	{
		STATE_RUN_AWAY,
		STATE_PATROL,
		STATE_FOLLOW,
		STATE_ATTACK,
		STATE_STAY,
		STATE_NONE
	};

	/**
	 * @brief Types d'ai
	 * 
	 * Passive Harmless :	n'attaque pas et se sauve si attaqu�
	 * Passive :			n'attaque pas mais attaque si attaqu�
	 * Aggressive :			attaque
	 */
	enum AiType
	{
		TYPE_PASSIVE_HARMLESS,
		TYPE_PASSIVE,
		TYPE_AGGRESSIVE
	};

	/**
	 * Constructeur
	 * @param type		AiType
	 * @param npc		Pointeur vers le npc que l'ai controle
	 * @param player	Pointeur vers le joueur
	 */
	AI(AiType type, Npc* npc, Player* player = 0);

	/**
	 * Deconstructeur
	 */
	virtual ~AI();

	/**
	 * @return true Si l'�tat a changer
	 */
	virtual bool StateChanged() const;

	/**
	 * @return l'�tat courant
	 */
	virtual State GetState() const;

	/**
	 * Boucle principal de l'ai
	 * D�termine le state et ce que l'objet control� par
	 * l'ai doit faire.
	 * @param elapsedTime Game loop elapsed time
	 */
	virtual void Process(float elapsedTime);

	/**
	 * Calcul une nouvelle destination de patrol
	 */
	virtual void Patrol();
	virtual void RunAway();
	virtual void Attack();
	virtual void Follow();
	virtual void Stay();

protected:
	virtual bool CheckVision(const Vector3f& pos) const;

	/**
	 * Detecte s'il y a collision
	 * @param pos position a tester
	 */
	virtual bool CheckCollision(Vector3f& pos) const;

	/**
	 * Regarde si le joueur est d�tect�
	 * @param detectionRange distance max pour �tre vu
	 * @param enableLOS si le joueur doit �tre visible
	 */
	virtual bool CheckPlayer(float detectionRange, bool enableLOS = true);
	virtual bool CheckAttack();

	/**
	 * @return distance entre le joueur et le npc
	 */
	Vector3f PlayerDistance();
protected:
	Player* m_player;
	State m_currentState;
	State m_lastState;
	AiType m_type;
	Npc* m_npc;

	Vector3f* m_patrolDestination;
	Vector3f m_posIni;

	bool m_stateChanged;
};

#endif