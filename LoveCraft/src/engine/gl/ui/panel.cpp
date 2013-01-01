#include "panel.h"


Panel::Panel() : Control(CTRLTYPE_PANEL), m_ctrlNbr(0), m_capacity(0)/*, m_controls(Array<Control*>(1, 0)), m_controlNbr(1)*/
{
	m_controls = 0;
}

Panel::Panel(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, 
			 const unsigned short controlnbr, const std::string& name) : Control(CTRLTYPE_PANEL, 
			 parent, position, size, texture, name), m_ctrlNbr(0), m_capacity(controlnbr)/*, m_controls(Array<Control*>(controlnbr, 0))*/
{
	m_controls = new Control*[controlnbr];
	for (unsigned short i = 0; i < controlnbr; i++)
		m_controls[i] = 0;
}

Panel::~Panel()
{
	
}

//void Panel::AddControl(Control* control)
//{
//	m_controls.Set(m_ctrlNbr, control);
//	//Incrémentation du nombre de controles présents
//	m_ctrlNbr++;
//}
//Control* Panel::GetControlById(unsigned short index) const
//{
//	assert(index < m_capacity);
//	return m_controls.Get(index);
//}
//Control* Panel::GetControlByName(const std::string& name) const
//{
//	Control* ctrl = 0;
//	for (unsigned short i = 0; i < m_controlNbr; i++)
//	{
//		if (m_controls.Get(i)->Name() == name)
//		{
//			ctrl = m_controls.Get(i);
//			break;
//		}
//	}
//	return ctrl;
//}

void Panel::AddControl(Control* control)
{
	m_controls[m_ctrlNbr] = control;
	//Incrémentation du nombre de controles présents
	m_ctrlNbr++;
}
Control* Panel::GetControlById(unsigned short index) const
{
	assert(index < m_capacity);
	return m_controls[index];
}
Control* Panel::GetControlByName(const std::string& name) const
{
	Control* ctrl = 0;
	for (unsigned short i = 0; i < m_capacity; i++)
	{
		if (m_controls[i]->Name() == name)
		{
			ctrl = m_controls[i];
			break;
		}
	}
	return ctrl;
}

void Panel::Render()
{
	Control::Render();
	RenderAllControls();
}

//void Panel::RenderAllControls()
//{
//	if (m_ctrlNbr != 0)
//	{
//		for (unsigned short i = 0; i < m_ctrlNbr; i++)
//			m_controls.Get(i)->Render();
//	}
//}

void Panel::RenderAllControls()
{
	if (m_ctrlNbr != 0 && m_controls != 0)
	{
		for (unsigned short i = 0; i < m_ctrlNbr; i++)
			m_controls[i]->Render();
	}
}

Panel& Panel::operator=(const Panel& p)
{
	m_capacity = p.m_capacity;
	m_ctrlNbr = m_ctrlNbr;
	m_name = p.m_name;
	m_parent = p.m_parent;
	m_controls = p.m_controls;
	m_position = p.m_position;
	m_size = p.m_size;
	m_texture = p.m_texture;
	m_type = p.m_type;
	m_visible = p.m_visible;

	return *this;
}
