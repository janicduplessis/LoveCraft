#ifndef NPC_H__
#define NPC_H__

#include "define.h"
#include "engine/gl/model.h"
#include "engine/ai.h"

#include "util/array.h"
#include "util/vector3.h"
#include "util/vector2.h"
#include "util/quaternion.h"

class AI;

class Npc
{
public:
	Npc(const Vector3f& pos = Vector3f(0,0,0));
	virtual ~Npc();

	virtual void Init();
	virtual void Update() = 0;
	virtual void Render() const;
	virtual void Move(const Vector3f& destination, float elapsedTime);

	Vector3f Position() const;

protected:
	Vector3f m_pos;
	Vector3f m_speed;
	Vector3f m_acceleration;
	Quaternion m_rot;
	float m_maxRot;

	AI* m_ai;
	Model m_model;
};

#endif // !NPC_H__
