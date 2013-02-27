#include "theme.h"
#include "interfaceinfos.h"
#include "uiimage.h"
#include "fontcolor.h"

Theme** Theme::m_themes = 0;

Theme::Theme() : m_backButton(0), m_backListbox(0), m_backPanel(0), m_backTextbox(0)
{
}


Theme::~Theme()
{
}

void Theme::InitThemes()
{
	m_themes[THEME_DEFAULT]->Init("default");
	m_themes[THEME_DEFAULT]->SetText(THEME_DEFAULT_CHAR_H, THEME_DEFAULT_CHAR_W, THEME_DEFAULT_CHAR_I, 
		THEME_DEFAULT_ITALIC, TEXTCOLOR_WHITE);
	m_themes[THEME_DEFAULT]->SetBackgrounds(CUSTIMAGE_LAST, CUSTIMAGE_LAST, CUSTIMAGE_LAST, CUSTIMAGE_LAST);

	m_themes[THEME_MAINMENU]->Init("mainmenu");
	m_themes[THEME_MAINMENU]->SetText(THEME_MAIN_MENU_CHAR_H, THEME_MAIN_MENU_CHAR_W, THEME_MAIN_MENU_CHAR_I,
		THEME_MAIN_MENU_ITALIC, Info::Get().GetFontColor(THEME_MAIN_MENU_COLOR), Info::Get().GetTexture(THEME_MAIN_MENU_BACK));
	m_themes[THEME_MAINMENU]->SetBackgrounds(UIImage::Get(CUSTIMAGE_MENU_BACKGROUND)

		m_themes[THEME_CONSOLE]->Init("console");
	m_themes[THEME_CONSOLE]->SetText(THEME_CONSOLE_CHAR_H, THEME_CONSOLE_CHAR_W, THEME_CONSOLE_CHAR_I,
		THEME_CONSOLE_ITALIC, Info::Get().GetFontColor(THEME_CONSOLE_COLOR), Info::Get().GetTexture(THEME_CONSOLE_BACK));
	m_themes[THEME_PLAYER_VALUES]->Init("playervalues");
	m_themes[THEME_PLAYER_VALUES]->SetText(THEME_PLAYERVALUES_CHAR_H, THEME_PLAYERVALUES_CHAR_W, THEME_PLAYERVALUES_CHAR_I,
		THEME_PLAYERVALUES_ITALIC, Info::Get().GetFontColor(THEME_PLAYERVALUES_COLOR));
}

void Theme::Init(string name)
{
	m_name = name;
}

void Theme::SetText(float height, float width, float interval, bool italic, COLOR color)
{
	m_charH = height;
	m_charW = width;
	m_charI = interval;
	m_italicText = italic;
	m_foreColor = FontColor::Get(color);
}

Theme* Theme::GetTheme(ThemeSet theme)
{
	if (!m_themes)
	{
		m_themes = new Theme*[THEME_LAST];
		for (uint8 i = 0; i < THEME_LAST; i++)
			m_themes[i] = new Theme();
		InitThemes();
	}
	return m_themes[theme];
}

void Theme::SetBackgrounds(IMAGE mainpanel, IMAGE button, IMAGE textbox, IMAGE listbox)
{
	if (mainpanel != CUSTIMAGE_LAST)
		m_backPanel = UIImage::Get(mainpanel);
	if (button != CUSTIMAGE_LAST)
		m_backButton = UIImage::Get(button);
	if (textbox != CUSTIMAGE_LAST)
		m_backTextbox = UIImage::Get(textbox);
	if (listbox != CUSTIMAGE_LAST)
		m_backListbox = UIImage::Get(listbox);
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
