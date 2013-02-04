#include "singletext.h"


SingleText::SingleText(CONTROLTYPE type) : Textual(type)
{
	m_label = new Label();
}


SingleText::~SingleText()
{
	delete m_label;
}

void SingleText::InitTextual(Texture* color, bool italic, float charHeight, float charWidth, float charinterval)
{
	Textual::InitTextual(color, italic, charHeight, charWidth, charinterval);
	m_label->InitControl("textlabel", this);
	m_label->InitLocalizable(GetPosition(), Size(), 0);
	m_label->InitTextual(color, italic, charHeight, charWidth, charinterval);
	m_label->SetDocking(TEXTDOCK_MIDDLECENTER);
}

void SingleText::InitTextual(ThemeSet theme)
{
	Theme* t = Theme::GetTheme(theme);
	InitTextual(t->GetForeColor(), t->GetItalic(), t->GetHeight(), 
		t->GetWidth(), t->GetInterval());
}

#pragma region Label properties

void SingleText::SetMsg(string message)
{
	Textual::SetMsg(message);
	m_label->SetMsg(message);
}
void SingleText::SetColor(Texture* color)
{
	Textual::SetColor(color);
	m_label->SetColor(color);
}
void SingleText::SetItalic()
{
	Textual::SetItalic();
	m_label->SetItalic();
}
void SingleText::RemoveItalic()
{
	Textual::RemoveItalic();
	m_label->RemoveItalic();
}
void SingleText::SetCharHeight(float height)
{
	Textual::SetCharHeight(height);
	m_label->SetCharHeight(height);
}
void SingleText::SetCharWidth(float width)
{
	Textual::SetCharWidth(width);
	m_label->SetCharWidth(width);
}
void SingleText::SetCharInterval(float interval)
{
	Textual::SetCharInterval(interval);
	m_label->SetCharInterval(interval);
}
void SingleText::SetOffset(Point offset)
{
	m_label->SetOffset(offset);
}
void SingleText::SetDocking(Docking dock)
{
	m_label->SetDocking(dock);
}
Docking SingleText::GetDocking() const
{
	return m_label->GetDocking();
}
bool SingleText::IsDocking(Docking dock) const
{
	return m_label->IsDocking(dock);
}
void SingleText::UseNextDocking()
{
	m_label->UseNextDocking();
}
#pragma endregion