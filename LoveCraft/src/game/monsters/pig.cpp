#include "pig.h"

Pig::Pig() : Animal()
{

}

Pig::~Pig()
{
	delete m_ai;
}

void Pig::Init(Player* player)
{
	m_model.Load(MODEL_PATH "pig.lcm");
	m_model.Translate(0,-1.3,0);
	m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
	m_speed = Vector3f(0,0,1);
}
