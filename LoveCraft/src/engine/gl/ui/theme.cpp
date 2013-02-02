#include "theme.h"


Theme::Theme()
{
}


Theme::~Theme()
{
}

Theme* Default()
{
	Theme* theme = new Theme();
	theme->Init("default");
	theme->Set(18.f, 18.f, 0.6f, false, 0, 0);
	return theme;
}

void Theme::Init(string name)
{
	m_name = name;
}
void Theme::Set(float height, float width, float interval, bool italic, Texture* color, Texture* background)
{
	m_charH = height;
	m_charW = width;
	m_charI = interval;
	m_italicText = italic;
	m_foreColor = color;
	m_backColor = background;
}

string Theme::GetName() const
{
	return m_name;
}
float Theme::GetHeight() const
{
	return m_charH;
}
float Theme::GetWidth() const
{
	return m_charW;
}
float Theme::GetInterval() const
{
	return m_charI;
}
bool Theme::GetItalic() const
{
	return m_italicText;
}
Texture* Theme::GetForeColor() const
{
	return m_foreColor;
}
Texture* Theme::GetBackColor() const
{
	return m_backColor;
}