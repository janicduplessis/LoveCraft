#include "textual.h"
#include "../../fontcolor.h"
#include "../../uiimage.h"

Textual::Textual(CONTROLTYPE type) : Localizable(type, CTRLGENERIC_TEXTUAL), m_message(""), m_italic(false), 
	m_charHeight(0.f), m_charWidth(0.f), m_charInterval(0.f), m_fontColor(0)
{
	InitTextual(THEME_DEFAULT);
}

Textual::~Textual()
{
}

#pragma region Class funtions

void Textual::InitTextual(COLOR color, bool italic, float charHeight, float charWidth, float charinterval, IMAGE background)
{
	m_fontColor = FontColor::Get(color);
	m_italic = italic;
	m_charHeight = charHeight;
	m_charWidth = charWidth;
	m_charInterval = charinterval;
	m_background = background != CUSTIMAGE_LAST ? UIImage::Get(background) : 0;
}

#pragma endregion

// Propriétés

#pragma region Message

void Textual::SetMsg(string message)
{
	m_message = message;
}
void Textual::AddMsg(string value)
{
	static std::ostringstream ss;
	ss << m_message << value;
	m_message = ss.str();
	ss.str("");
}
string Textual::GetMsg() const
{
	return m_message;
}
bool Textual::IsMsg(string message) const
{
	return m_message == message;
}
bool Textual::HasMsg() const
{
	return m_message != "";
}
void Textual::ResetMsg()
{
	m_message = "";
}

#pragma endregion

#pragma region Font color

void Textual::SetColor(COLOR color)
{
	m_fontColor = color;
}
COLOR Textual::GetColor() const
{
	return m_fontColor;
}
bool Textual::IsColor(COLOR color) const
{
	return m_fontColor == FontColor::Get(color);
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
void Textual::AddCharHeight(float value)
{
	m_charHeight += value;
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
void Textual::AddCharWidth(float value)
{
	m_charWidth += value;
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
void Textual::AddCharInterval(float value)
{
	m_charInterval += value;
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