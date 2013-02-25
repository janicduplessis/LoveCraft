#include "container.h"


Container::Container(CONTROLTYPE type) : Localizable(type, CTRLGENERIC_CONTAINER), 
	m_ctrlNbr(0), m_controls(0)
{
}

Container::~Container()
{
	delete [] m_controls;
}

void Container::AddControl(Localizable* control)
{
	if (!control)
		return;
	Localizable** controls = m_controls;
	m_controls = new Localizable*[++m_ctrlNbr];
	for (uint8 i = 0; i < m_ctrlNbr - 1; i++)
		m_controls[i] = controls[i];
	m_controls[m_ctrlNbr - 1] = control;
	delete [] controls;
}
Localizable* Container::GetControlById(uint8 index) const
{
	assert(index < m_ctrlNbr);
	return m_controls[index];
}
Localizable* Container::GetControlByName(const string& name) const
{
	Localizable* ctrl = 0;
	for (uint8 i = 0; i < m_ctrlNbr; i++)
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

Localizable* Container::GetTopControl(int x, int y)
{
	for (uint8 i = 0; i < m_ctrlNbr; i++)
	{
		Localizable* current = m_controls[i];

		if (current->IsGenType(CTRLGENERIC_CONTAINER))
		{
			Localizable* ref = dynamic_cast<Container*>(current)->GetTopControl(x, y);
			if (ref)
				return ref;
			if (current->IsWithinRange(x, y) && current->IsVisible())
				return current;
		}
		if (current->IsWithinRange(x, y) && current->IsVisible())
			return current;
	}
	return 0;
}
