#include "button.h"
#include "../label.h"

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

#pragma region Events

bool Button::MousePressEvents(int x, int y)
{
	if (m_clicked)
		return true;

	if (IsVisible() && IsEnabled())
	{
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

#pragma endregion

// Propriétés

#pragma region Clicked

bool Button::IsHeld() const
{
	return m_clicked;
}

#pragma endregion