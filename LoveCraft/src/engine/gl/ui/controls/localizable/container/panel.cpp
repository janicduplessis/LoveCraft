#include "panel.h"

Panel::Panel() : Container(CTRLTYPE_PANEL)
{
}

Panel::~Panel()
{

}

void Panel::Init(Texture* background)
{
	m_background = background;
}

void Panel::DrawingBindTexture()
{
	m_background->Bind();
}

void Panel::Render()
{
	if (m_visible)
	{
		if (m_background)
			DrawSquare();
		if (m_controls != 0)
		{
			for (uint8 i = 0; i < m_ctrlNbr; i++)
				m_controls[i]->Render();
		}
	}
}
