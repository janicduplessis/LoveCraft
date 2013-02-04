#include "panel.h"

Panel::Panel() : Container(CTRLTYPE_PANEL)
{
}

Panel::~Panel()
{

}

void Panel::Render()
{
	if (IsVisible())
	{
		if (GetBackground())
			DrawSquare();
		if (m_controls)
		{
			for (uint8 i = 0; i < m_ctrlNbr; i++)
				m_controls[i]->Render();
		}
	}
}
