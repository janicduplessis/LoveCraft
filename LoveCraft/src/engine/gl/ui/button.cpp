#include "button.h"

Button::Button() : m_clicked(false), m_label(0)
{

}

Button::Button(Control* parent, const Vector2i& position, const Vector2i &size, Texture* background, 
			   Texture* textColor, const std::string& text, const std::string& name) : 
	Control(CTRLTYPE_PICTUREBOX, parent, position, size, background, name)
{
	if (text != "")
		m_label = new Label(this, Vector2i(), textColor, text, Label::TEXTDOCK_MIDDLECENTER, false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2i(), "label_");
}

Button::~Button()
{
	delete m_label;
}

bool Button::isClicked( int x, int y )
{
	if (m_clicked)
		return true;

	Vector2i pos = AbsolutePosition();
	if (x >= pos.x && x <= pos.x + m_size.x && y >= pos.y && y <= pos.y + m_size.y) {
		OnClick.Notify(this);
		m_clicked = true;
		Press();
		return true;
	}
	return false;
}

void Button::Release()
{
	m_clicked = false;
}

void Button::Press()
{
	m_clicked = true;
}
