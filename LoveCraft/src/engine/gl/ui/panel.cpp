#include "panel.h"


Panel::Panel() : Control(CTRLTYPE_PANEL), m_index(0)
{
	//m_controls = new Control[m_controlNbr];
}

Panel::Panel(Vector2i parent, Vector2i &position, Vector2i &size, Texture* texture, 
			 const unsigned short controlnbr, const std::string& name) : Control(CTRLTYPE_PANEL, 
			 parent, position, size, texture, name), m_index(0), m_controlNbr(controlnbr)
{
	//m_controls = new Control[m_controlNbr];
}

Panel::~Panel()
{
	//delete [] m_controls;
}

//void Panel::AddControl(const Control& control)
//{
//	//Tente d'ajouter le control à la liste de controles
//	if (m_index < m_controlNbr)
//	{
//		//Assignation réussie
//		m_controls[m_index] = control;
//		//Incrémentation du nombre de controles présents
//		m_index++;
//	}
//	//Si la liste est pleine
//	else
//	{
//		//Augmente le nombre de controle disponible
//		m_controlNbr++;
//		//Redimensionne le pointeur
//		Resize(m_controlNbr);
//		//Retente l'ajout du control
//		AddControl(control);
//	}
//}
//Control& Panel::GetControlById(unsigned short index) const
//{
//	assert(index < m_controlNbr);
//	return m_controls[index];
//}
//Control& Panel::GetControlByName(const std::string& name) const
//{
//	for (unsigned short i = 0; i < m_controlNbr; i++)
//	{
//		if (m_controls[i].Name() == name)
//			return m_controls[i];
//	}
//	return Control();
//}

void Panel::Render()
{
	RenderSquare(AbsolutePosition(), m_size, m_texture);
	//RenderAllControls();
}

void Panel::RenderAllControls()
{
	//for (unsigned short i = 0; i < m_controlNbr; i++)
	//	m_controls[i].Render();
}

//private

void Panel::Resize(unsigned short newNbr)
{
	//Control* data = new Control[newNbr];
	//for (unsigned short i = 0; i < newNbr - 1; i++)
	//	data[i] = m_controls[i];
	//m_controls = data;
	//delete [] data;
}