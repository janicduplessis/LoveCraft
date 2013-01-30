#include "textbox.h"

Textbox::Textbox() : Control(CTRLTYPE_TEXTBOX), IText(), m_hasFocus(false)
{
	m_label = new Label();
}

Textbox::~Textbox()
{
	delete m_label;
}

void Textbox::Init(Vector2i &offset)
{
	m_label->CtrlInit(this, Vector2i((int)offset.x, (int)offset.y), Vector2i(), 0, "message");
	m_label->TextInit("", m_fontColor, m_italic, m_charHeight, m_charWidth, m_charInterval);
	m_label->Init(Label::TEXTDOCK_MIDDLELEFT, Vector2f(offset.x, offset.y));
}

bool Textbox::HasFocus() const
{
	return m_hasFocus;
}
void Textbox::SetFocus(const bool value)
{
	m_hasFocus = value;
}
void Textbox::SetMessage(string text)
{
	m_label->SP(PropString::PROPSTR_TEXT, text);
}
string Textbox::GetMsg() const
{
	return m_label->GP(PropString::PROPSTR_TEXT);
}

Label* Textbox::TLabel()
{
	return m_label;
}

void Textbox::Render()
{
	if (m_hasFocus)
	{
		Control::Render();
		m_label->Render();
	}
}