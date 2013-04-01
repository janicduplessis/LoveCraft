#include "control.h"

Control::Control(CONTROLTYPE type) : Object(), m_type(type), m_enabled(true)
{
}

Control::~Control()
{
}

#pragma region Class funtions

void Control::InitControl(string name)
{
	m_name = name;
}

#pragma endregion

// Propriétés

#pragma region Enabled

bool Control::IsEnabled() const
{
	return m_enabled;
}
void Control::Enable()
{
	if (m_enabled)
		return;
	m_enabled = true;
}
void Control::Disable()
{
	if (!m_enabled)
		return;
	m_enabled = false;
}

#pragma endregion

#pragma region Type

CONTROLTYPE Control::GetType() const
{
	return m_type;
}
bool Control::IsType(CONTROLTYPE type) const
{
	return m_type == type;
}

#pragma endregion