#include "panel.h"

Panel::Panel() : Container(CTRLTYPE_PANEL)
{
}

Panel::~Panel()
{

}

void Panel::Init(Texture* background)
{

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
		Container::Render();
	}
}
