#ifndef NPC_H__
#define NPC_H__

#include "define.h"
#include "spell.h"
#include "engine/gl/model.h"
#include "engine/info.h"
#include "engine/ai.h"

#include "util/array.h"
#include "util/vector3.h"
#include "util/vector2.h"
#include "util/quaternion.h"

class AI;

class Npc
{
public:
	enum Type
	{
		HOSTILE,
		FRIENDLY,
		NEUTRAL
	};

	Npc(const Vector3f& pos = Vector3f(0,0,0));
	virtual ~Npc();

	virtual void Init(Player* player = 0) = 0;
	virtual void Update(float elapsedTime);
	virtual void Render() const;
	virtual void Move(const Vector3f& destination, float elapsedTime);

	Vector3f Position() const;
	void SetPosition(const Vector3f& pos);
	bool CheckCollision(const Vector3f& pos) const;
protected:
	Vector3f m_pos;
	Vector3f m_speed;
	Quaternion m_rot;
	float m_maxRot;
	float m_speedGravity;

	Type m_type;
	Spell* m_spells;

	AI* m_ai;
	Model m_model;
};

#endif // !NPC_H__
