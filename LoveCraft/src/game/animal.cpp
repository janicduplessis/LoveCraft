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
	//Vitesse par défaut
	m_speed = Vector3f(0,0,1);
	switch (type)
	{
	case Animal::ANL_GRD_PIG:
		m_model.Load(MODEL_PIG);
		m_model.Translate(0,-1.3f,0);
		break;
	case Animal::ANL_GRN_DEER_M:
		m_model.Load(MODEL_DEER_M);
		m_model.Translate(0,-1.25f,0);
		break;
	case Animal::ANL_GRD_BWIDOW:
		m_model.Load(MODEL_BWIDOW);
		m_model.Translate(0,-1.3f,0);
		m_speed.z = 5.0f;
		break;
	case Animal::ANL_GRD_ARMDILLO:
		m_model.Load(MODEL_ARMDILLO);
		m_model.Translate(0,-1.15f,0);
		m_model.Scale(2.7f, 2.7f, 2.7f);
		m_speed.z = 0.5f;
		break;
	case Animal::ANL_GRD_RHINO:
		m_model.Load(MODEL_RHINO);
		m_model.Translate(0,-1.15f,0);
		m_model.Scale(0.7f, 0.7f, 0.7f);
		break;
	case Animal::ANL_GRD_ARACHNID:
		m_model.Load(MODEL_ARACHNID);
		m_model.Translate(0,-1.3f,0);
		m_speed.z = 2.0f;
		break;
	}
	m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
	m_initialized = true;
}

void Animal::Init(Player* player)
{
	Init(ANL_GRD_PIG, player);
}