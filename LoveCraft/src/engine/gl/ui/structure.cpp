#include "structure.h"


Structure::Structure() : m_initialized(false)
{
}


Structure::~Structure()
{
}

void Structure::Init()
{
	m_initialized = true;
}