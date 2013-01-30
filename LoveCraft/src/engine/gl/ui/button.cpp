#include "button.h"

Button::Button() : Control(CTRLTYPE_BOUTON), IText(), m_clicked(false)
{
	m_label = new Label();
}

Button::~Button()
{
	delete m_label;
}

void Button::Init()
{
	m_label->PreInit(this, Vector2i(), Vector2i(), m_texture, "label");
	m_label->Init(Label::TEXTDOCK_MIDDLECENTER, Vector2f());
	m_label->TextInit(text, false, LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I);
}

void Button::Render()
{
	Control::Render(m_texture);
	if (GetText() != "" && m_visible)
		m_label->Render();
}

bool Button::MousePressEvents( int x, int y )
{
	if (m_clicked)
		return true;

	if (m_visible && m_enabled)
	{
		Vector2i& pos = AbsolutePosition();
		if (x >= pos.x && x <= pos.x + m_size.x && y >= pos.y && y <= pos.y + m_size.y) {
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

void Button::SetTextTo(std::string text)
{
	m_label->SP(Label::PropString::PROPSTR_TEXT, text);
}
std::string Button::GetText() const
{
	return m_label->GP(Label::PropString::PROPSTR_TEXT);
}

Label* Button::TLabel()
{
	return m_label;
}