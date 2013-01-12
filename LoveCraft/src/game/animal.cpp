#include "animal.h"


Animal::Animal() : Npc(), m_initialized(false)
{
}


Animal::~Animal()
{
	delete m_ai;
}

bool Animal::Initialized() const
{
	return m_initialized;
}

void Animal::Init(AnimalType type, Player* player)
{
	switch (type)
	{
	case Animal::ANL_GRD_PIG:
		m_model.Load(MODEL_PIG);
		m_model.Translate(0,-1.3f,0);
		break;
	case Animal::ANL_GRD_BWIDOW:
		m_model.Load(MODEL_BWIDOW);
		m_model.Translate(0,-1.3f,0);
		break;
	case Animal::ANL_GRD_ARMDILLO:
		m_model.Load(MODEL_ARMDILLO);
		m_model.Translate(0,-1.3f,0);
		break;
	case Animal::ANL_GRD_RHINO:
		m_model.Load(MODEL_RHINO);
		m_model.Translate(0,-1.3f,0);
		break;
	case Animal::ANL_GRD_ARACHNID:
		m_model.Load(MODEL_ARACHNID);
		m_model.Translate(0,-1.3f,0);
		break;
	}
	m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
	m_speed = Vector3f(0,0,1);
	m_initialized = true;
}

void Animal::Init(Player* player)
{
	Init(ANL_GRD_PIG, player);
}