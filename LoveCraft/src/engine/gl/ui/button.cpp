#include "button.h"

Button::Button() : m_clicked(false), m_label(0)
{

}

Button::Button(Control* parent, const Vector2i& position, const Vector2i &size, Texture* background, 
			   Texture* textColor, const std::string& text, const std::string& name) : 
Control(CTRLTYPE_BOUTON, parent, position, size, background, name), m_clicked(false), m_label(0)
{
	m_label = new Label(this, Vector2i(), textColor, text, Label::TEXTDOCK_MIDDLECENTER, false, 
		LBL_GENERIC_CHAR_H, LBL_GENERIC_CHAR_W, LBL_GENERIC_CHAR_I, Vector2f(), "label_");
	m_label->SetProperty(Label::PropString::PROPSTR_TEXT, text);
}

Button::~Button()
{
	delete m_label;
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

	if (m_visible)
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
	m_label->SetProperty(Label::PropString::PROPSTR_TEXT, text);
}
std::string Button::GetText() const
{
	return m_label->GetProperty(Label::PropString::PROPSTR_TEXT);
}

Label* Button::TLabel()
{
	return m_label;
}