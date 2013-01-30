#include "panel.h"

// Avant l'héritage multiple
//Panel::Panel() : Control(CTRLTYPE_PANEL), m_ctrlNbr(0), m_capacity(0), m_controls(0)
//{
//}

Panel::Panel() : Control(CTRLTYPE_PANEL), IContainer(1)
{
}

// Avant l'héritage multiple
//Panel::Panel(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, 
//			 const unsigned short controlnbr, const std::string& name) : Control(CTRLTYPE_PANEL, 
//			 parent, position, size, texture, name), m_ctrlNbr(0), m_capacity(controlnbr)
//{
//	m_controls = new Control*[controlnbr];
//	for (unsigned short i = 0; i < controlnbr; i++)
//		m_controls[i] = 0;
//}

Panel::Panel(Control* parent, Vector2i &position, Vector2i &size, Texture* texture, 
			 const unsigned short controlnbr, const std::string& name) : Control(CTRLTYPE_PANEL, 
			 parent, position, size, texture, name), IContainer(controlnbr)
{
}

Panel::~Panel()
{

}

// Avant l'héritage multiple
//void Panel::AddControl(Control* control)
//{
//	m_controls[m_ctrlNbr] = control;
//	//Incrémentation du nombre de controles présents
//	m_ctrlNbr++;
//}

void Panel::Render()
{
	Control::Render();
	if (m_visible)
		RenderAllControls();
}

// Avant l'héritage multiple
//void Panel::RenderAllControls()
//{
//	if (m_ctrlNbr != 0 && m_controls != 0)
//	{
//		for (unsigned short i = 0; i < m_ctrlNbr; i++)
//			m_controls[i]->Render();
//	}
//}
