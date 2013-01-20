#include "interface.h"


Interface::Interface() : m_loaded(false), m_val(0)
{
}


Interface::~Interface()
{
}

void Interface::DeInit()
{
	m_val = 0;
}

void Interface::Init(ValuesInterface* values)
{
	m_loaded = true;
	m_val = values;
	InitControls();
}