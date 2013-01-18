#include "interface.h"


Interface::Interface() : m_loaded(false)
{
}


Interface::~Interface()
{
}

void Interface::Init(Engine* engine)
{
	m_loaded = true;
	m_engine = engine;
	InitControls();
}