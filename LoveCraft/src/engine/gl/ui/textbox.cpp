#include "textbox.h"


Textbox::Textbox(Control* parent, Vector2i position, Vector2i size, Texture* color, Texture* back, 
				 Label::Docking dock, bool italic, float charWidth, 
				 float charinterval, Vector2f offset, const std::string& name) :
Control(CTRLTYPE_TEXTBOX, parent, position, size, back, name)
{
	m_label = new Label(this,
		Vector2i((int)offset.x, (int)offset.y), color, "", dock, italic, (size.y - 2 - offset.y * 2), 
		charWidth, charinterval, Vector2f(), "message");
}


Textbox::~Textbox()
{
}

bool Textbox::HasFocus() const
{
	return m_hasFocus;
}
void Textbox::SetFocus(const bool value)
{
	m_hasFocus = value;
}
void Textbox::SetMessage(std::string text)
{
	m_label->SetProperty(PropString::PROPSTR_TEXT, text);
}
std::string Textbox::GetMsg() const
{
	return m_label->GetProperty(PropString::PROPSTR_TEXT);
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