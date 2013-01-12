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
		m_model.Load(NPC_PIG_MODEL);
		m_model.Translate(0,-1.1f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_BWIDOW:
		m_model.Load(NPC_BWIDOW_MODEL);
		m_model.Translate(0,-1.3f,0);
		m_speed.z = 5.0f;
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_ARMDILLO:
		m_model.Load(NPC_ARMDILLO_MODEL);
		m_model.Translate(0,-1.15f,0);
		m_model.Scale(2.7f, 2.7f, 2.7f);
		m_speed.z = 0.5f;
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_RHINO:
		m_model.Load(NPC_RHINO_MODEL);
		m_model.Translate(0,-1.15f,0);
		m_model.Scale(0.7f, 0.7f, 0.7f);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_ARACHNID:
		m_model.Load(NPC_ARACHNID_MODEL);
		m_model.Translate(0,-1.3f,0);
		m_speed.z = 2.0f;
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_DEER_M:
		m_model.Load(NPC_DEER_M_MODEL);
		m_model.Translate(0,-1.25f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_COYOTE:
		m_model.Load(NPC_COYOTE_MODEL);
		m_model.Scale(2,2,2);
		m_model.Translate(0,-1.14f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_DONKEY:
		m_model.Load(NPC_DONKEY_MODEL);
		m_model.Scale(2,2,2);
		m_model.Translate(0,-1.25f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_HUSKY:
		m_model.Load(NPC_HUSKY_MODEL);
		m_model.Scale(2, 2, 2);
		m_model.Translate(0,-1.14f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_HYENA:
		m_model.Load(NPC_HYENA_MODEL);
		m_model.Scale(1.5f, 1.5f, 1.5f);
		m_model.Translate(0,-1.1f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_REDFOX:
		m_model.Load(NPC_REDFOX_MODEL);
		m_model.Scale(2, 2, 2);
		m_model.Translate(0,-1.138f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_SHEEP:
		m_model.Load(NPC_SHEEP_MODEL);
		m_model.Translate(0,-1.139f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_CATBLK:
		m_model.Load(NPC_CATBLK_MODEL);
		m_model.Scale(3, 3, 3);
		m_model.Translate(0,-1.14f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_BISON:
		m_model.Load(NPC_BISON_MODEL);
		m_model.Translate(0,-1.25f,0);
		m_model.Scale(3, 3, 3);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_TORTOISE:
		m_model.Load(NPC_TORTOISE_MODEL);
		m_model.Translate(0,-1.25f,0);
		m_model.Scale(3, 3, 3);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_ALIGATR:
		m_model.Load(NPC_ALLIGATR_MODEL);
		m_model.Translate(0,-1.25f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_TRANTULA:
		m_model.Load(NPC_TRANTULA_MODEL);
		m_model.Scale(15, 15, 15);
		m_model.Translate(0,-1.2f,0);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_GRD_SCORPION:
		m_model.Load(NPC_SCORPION_MODEL);
		m_model.Translate(0,-1.2f,0);
		m_model.Scale(30, 30, 30);
		m_ai = new AI(AI::TYPE_PASSIVE_HARMLESS, this, player);
		break;
	case Animal::ANL_AIR_MONARCH:
		m_model.Load(NPC_MONARCH_MODEL);
		m_model.Translate(0,-1.3f,0);
		m_model.Scale(30, 30, 30);
		m_ai = new AI(AI::TYPE_PASSIVE_FLYING, this, player);
		m_flying = true;
		break;
	default:
		break;
	}
	m_initialized = true;
}

void Animal::Init(Player* player)
{
	Init(ANL_GRD_PIG, player);
}