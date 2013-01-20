#include "icontainer.h"


IContainer::IContainer(unsigned short capacity) : m_ctrlNbr(0), m_capacity(capacity)
{
	if (capacity != 0)
	{
		m_controls = new Control*[capacity];
		for (unsigned short i = 0; i < capacity; i++)
			m_controls[i] = 0;
	}
	else m_controls = 0;
}

IContainer::~IContainer()
{
}

void IContainer::AddControl(Control* control)
{
	m_controls[m_ctrlNbr] = control;
	//Incrémentation du nombre de controles présents
	m_ctrlNbr++;
}
Control* IContainer::GetControlById(unsigned short index) const
{
	assert(index < m_capacity);
	return m_controls[index];
}
Control* IContainer::GetControlByName(const std::string& name) const
{
	Control* ctrl = 0;
	for (unsigned short i = 0; i < m_capacity; i++)
	{
		if (m_controls[i]->GetProperty(Control::PROPSTR_NAME) == name)
		{
			ctrl = m_controls[i];
			break;
		}
	}
	return ctrl;
}


void IContainer::RenderAllControls()
{
	if (m_ctrlNbr != 0 && m_controls != 0)
	{
		for (unsigned short i = 0; i < m_ctrlNbr; i++)
			m_controls[i]->Render();
	}
}

IContainer& IContainer::operator=(const IContainer& p)
{
	m_capacity = p.m_capacity;
	m_ctrlNbr = m_ctrlNbr;
	m_controls = p.m_controls;

	return *this;
}
