﻿#include "button.h"

Button::Button() : SingleText(CTRLTYPE_BOUTON), m_clicked(false)
{
}

Button::Button(CONTROLTYPE type) : SingleText(type), m_clicked(false)
{

}

Button::~Button()
{
}

void Button::Render()
{
	DrawSquare();
	m_label->Render();
}

bool Button::MousePressEvents(int x, int y)
{
	if (m_clicked)
		return true;

	if (IsVisible() && IsEnabled())
	{
		Point& pos = AbsolutePosition();
		if (IsWithinRange(x, y))
		{
			OnClick.Notify(this);
			Press();
			return true;
		}
	}
	return false;
}

bool Button::MouseReleaseEvents(int x, int y)
{
	m_clicked = false;
	return true;
}

void Button::Press()
{
	m_clicked = true;
}