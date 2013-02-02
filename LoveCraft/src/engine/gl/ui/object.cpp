#include "object.h"


Object::Object() : m_name("")
{
}


Object::~Object()
{
}

std::string Object::GetName()
{
	return m_name;
}