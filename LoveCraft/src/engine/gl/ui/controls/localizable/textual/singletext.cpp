#include "singletext.h"
#include "label.h"

SingleText::SingleText(CONTROLTYPE type) : Textual(type)
{
	Text = new Label();
}

SingleText::~SingleText()
{
	delete Text;
}

#pragma region Class funtions

void SingleText::InitTextual(COLOR color, bool italic, float charHeight, float charWidth, float charinterval)
{
	Textual::InitTextual(color, italic, charHeight, charWidth, charinterval);
	Text->InitControl("textlabel");
	Text->InitLocalizable(GetPosition(), 0, this);
	Text->InitTextual(color, italic, charHeight, charWidth, charinterval);
	Text->SetDocking(TEXTDOCK_MIDDLECENTER);
}

void SingleText::InitTextual(ThemeSet theme)
{
	Theme* t = Theme::GetTheme(theme);
	InitTextual(t->GetForeColor(), t->GetItalic(), t->GetHeight(), 
		t->GetWidth(), t->GetInterval());
}

#pragma endregion

#pragma region Label properties

//void SingleText::SetMsg(string message)
//{
//	Textual::SetMsg(message);
//	m_label->SetMsg(message);
//}
//void SingleText::AddMsg(string value)
//{
//	static std::ostringstream ss;
//	ss << m_message << value;
//	Textual::SetMsg(ss.str());
//	m_label->SetMsg(ss.str());
//	ss.str("");
//}
//void SingleText::ResetMsg()
//{
//	Textual::ResetMsg();
//	m_label->ResetMsg();
//}
//void SingleText::SetColor(COLOR color)
//{
//	Textual::SetColor(color);
//	m_label->SetColor(color);
//}
//void SingleText::SetItalic()
//{
//	Textual::SetItalic();
//	m_label->SetItalic();
//}
//void SingleText::RemoveItalic()
//{
//	Textual::RemoveItalic();
//	m_label->RemoveItalic();
//}
//void SingleText::SetCharHeight(float height)
//{
//	Textual::SetCharHeight(height);
//	m_label->SetCharHeight(height);
//}
//void SingleText::AddCharHeight(float value)
//{
//	Textual::AddCharHeight(value);
//	m_label->AddCharHeight(value);
//}
//void SingleText::SetCharWidth(float width)
//{
//	Textual::SetCharWidth(width);
//	m_label->SetCharWidth(width);
//}
//void SingleText::AddCharWidth(float value)
//{
//	Textual::AddCharWidth(value);
//	m_label->AddCharWidth(value);
//}
//void SingleText::SetCharInterval(float interval)
//{
//	Textual::SetCharInterval(interval);
//	m_label->SetCharInterval(interval);
//}
//void SingleText::AddCharInterval(float value)
//{
//	Textual::AddCharInterval(value);
//	m_label->AddCharInterval(value);
//}
//void SingleText::SetOffset(Point offset)
//{
//	m_label->SetOffset(offset);
//}
//void SingleText::SetDocking(Docking dock)
//{
//	m_label->SetDocking(dock);
//}
//Docking SingleText::GetDocking() const
//{
//	return m_label->GetDocking();
//}
//bool SingleText::IsDocking(Docking dock) const
//{
//	return m_label->IsDocking(dock);
//}
//void SingleText::UseNextDocking()
//{
//	m_label->UseNextDocking();
//}

#pragma endregion