#include "pig.h"

Pig::Pig() : Npc()
{

}

Pig::~Pig()
{

}

void Pig::Init()
{
	m_model.Load(MODEL_PATH "pig.lcm");
}
