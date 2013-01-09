#include "button.h"

Button::Button() : m_clicked(false), m_label(0)
{

}

Button::~Button()
{
	delete m_label;
}

bool Button::isClicked( int x, int y )
{
	//if (m_clicked)
		//return true;

	Vector2i pos = AbsolutePosition();
	//if (x >= pos.x && x <= pos.x + m_size.x && y >= pos.y && y <= pos.y + m_size.y) {
		OnClick.Notify(this);
		m_clicked = true;
		Press();
		return true;
	//}
	//return false;
}

void Button::Release()
{
	m_clicked = false;
}

void Button::Press()
{
	m_clicked = true;
}
