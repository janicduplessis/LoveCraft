#include "interface.h"


Interface::Interface() : m_loaded(false) 
{
}


Interface::~Interface()
{
}

bool Interface::IsLoaded() const
{
	return m_loaded;
}