#include "pig.h"

Pig::Pig() : Npc()
{

}

Pig::~Pig()
{
	delete m_ai;
}

void Pig::Init(Player* player)
{
	m_model.Load(MODEL_PATH "pig.lcm");
	m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
	m_speed = Vector3f(0,0,1);
}
