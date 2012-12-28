#ifndef NPC_H__
#define NPC_H__

#include "util/array.h"
#include "util/vector3.h"
#include "util/vector2.h"
#include "define.h"
#include "engine/gl/model.h"
#include "engine/ai.h"

class Npc
{
public:
	Npc(const Vector3f& pos = Vector3f(0,0,0));
	virtual ~Npc();

	virtual void Init();
	virtual void Update();
	virtual void Render() const;

protected:
	Model m_model;
	Vector3f m_pos;
	AI m_ai;
};

#endif // !NPC_H__
