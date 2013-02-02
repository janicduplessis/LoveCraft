#include "textual.h"


Textual::Textual(CONTROLTYPE type) : Localizable(type), m_message(""), m_italic(false), m_charHeight(0.f), 
	m_charWidth(0.f), m_charInterval(0.f), m_fontColor(0)
{
	Theme* theme = Info::Get().GetTheme(THEME_DEFAULT);
	m_charHeight = theme->GetHeight();
	m_charWidth = theme->GetWidth();
	m_charInterval = theme->GetInterval();
	m_background = theme->GetBackColor();
	m_fontColor = theme->GetForeColor();
	m_italic = theme->GetItalic();
}


Textual::~Textual()
{
}

void Textual::Init(Texture* color, bool italic, float charHeight, float charWidth, float charinterval, Texture* background)
{
	m_fontColor = color;
	m_italic = italic;
	m_charHeight = charHeight;
	m_charWidth = charWidth;
	m_charInterval = charinterval;
	m_background = background;
}

void Textual::Init(Theme* theme)
{
	m_charHeight = theme->GetHeight();
	m_charWidth = theme->GetWidth();
	m_charInterval = theme->GetInterval();
	m_background = theme->GetBackColor();
	m_fontColor = theme->GetForeColor();
	m_italic = theme->GetItalic();
}

// Propriétés

#pragma region Message

void Textual::SetMsg(string message)
{
	m_message = message;
}
string Textual::GetMsg() const
{
	return m_message;
}
bool Textual::IsMsg(string message) const
{
	return m_message == message;
}

#pragma endregion

#pragma region Font color

void Textual::SetColor(Texture* color)
{
	m_fontColor = color;
}
Texture* Textual::GetColor() const
{
	return m_fontColor;
}
bool Textual::IsColor(Texture* color) const
{
	return m_fontColor == color;
}

#pragma endregion

#pragma region Italic

void Textual::SetItalic()
{
	m_italic = true;
}
void Textual::RemoveItalic()
{
	m_italic = false;
}
bool Textual::IsItalic() const
{
	return m_italic;
}

#pragma endregion

#pragma region Char height

void Textual::SetCharHeight(float height)
{
	m_charHeight = height;
}
float Textual::GetCharHeight() const
{
	return m_charHeight;
}
float Textual::IsCharHeight(float height) const
{
	float eps = 0.01f;
	return fabs(m_charHeight) - fabs(height) < eps;
}

#pragma endregion

#pragma region Char width

void Textual::SetCharWidth(float width)
{
	m_charWidth = width;
}
float Textual::GetCharWidh() const
{
	return m_charWidth;
}
float Textual::IsCharWidth(float width) const
{
	float eps = 0.01f;
	return fabs(m_charWidth) - fabs(width) < eps;
}

#pragma endregion

#pragma region Char interval

void Textual::SetCharInterval(float interval)
{
	m_charInterval = interval;
}
float Textual::GetCharInterval() const
{
	return m_charInterval;
}
float Textual::IsCharInterval(float interval) const
{
	float eps = 0.01f;
	return fabs(m_charInterval) - fabs(interval) < eps;
}

#pragma endregion