#include "control.h"

Control::Control(CONTROLTYPE type) : Object(), m_type(type), m_parent(0), m_enabled(true)
{
}

Control::~Control()
{

}

void Control::InitControl(string name, Control* parent)
{
	m_parent = parent;
	m_name = name;
}

#pragma region Enabled

bool Control::IsEnabled() const
{
	return m_enabled;
}
void Control::Enable()
{
	m_enabled = true;
}
void Control::Disable()
{
	m_enabled = false;
}

#pragma endregion

Control* Control::GetParent() const
{
	return m_parent;
}

CONTROLTYPE Control::GetType() const
{
	return m_type;
}
