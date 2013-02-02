#include "container.h"


Container::Container(CONTROLTYPE type) : Localizable(type), m_ctrlNbr(0), m_capacity(6)
{
}

Container::~Container()
{
	delete [] m_controls;
}

void Container::Init(uint8 capacity)
{
	m_capacity = capacity;
	m_controls = new Localizable*[capacity];
	for (uint8 i = 0; i < m_capacity; i++)
		m_controls[i] = 0;
}

void Container::Render()
{
	if (m_controls != 0)
	{
		for (uint8 i = 0; i < m_ctrlNbr; i++)
			m_controls[i]->Render();
	}
}

void Container::AddControl(Localizable* control)
{
	if (m_controls != 0)
	{
		assert(m_ctrlNbr < 6);
		m_controls[m_ctrlNbr] = control;
		m_ctrlNbr++;
	}
	else
	{
		Init(m_capacity);
		AddControl(control);
	}
}
Localizable* Container::GetControlById(uint8 index) const
{
	assert(index < m_capacity);
	return m_controls[index];
}
Localizable* Container::GetControlByName(const string& name) const
{
	Localizable* ctrl = 0;
	for (uint8 i = 0; i < m_capacity; i++)
	{
		if (m_controls[i]->GetName() == name)
		{
			ctrl = m_controls[i];
			break;
		}
	}
	// On s'assure que le controle existe
	if (ctrl == 0)
		assert(false);
	return ctrl;
}
