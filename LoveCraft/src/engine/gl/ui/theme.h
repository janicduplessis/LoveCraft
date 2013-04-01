#ifndef THEME_H__
#define THEME_H__

#include "define.h"
#include "enumerators.h"
#include "../texture.h"
#include "engine/info.h"
#include <cassert>
#include <string>
#include <iostream>

class Theme
{
public:
	Theme();
	~Theme();
	static Theme* GetTheme(ThemeSet theme);
	string GetName() const;

	float GetHeight() const;
	float GetWidth() const;
	float GetInterval() const;
	bool GetItalic() const;
	COLOR GetForeColor() const;

	IMAGE GetBackBackPanel() const;
	IMAGE GetBackButton() const;
	IMAGE GetBackTextbox() const;
	IMAGE GetBackListbox() const;
private:
	static Theme** m_themes;
	static void InitThemes();

	void Init(string name);
	string m_name;

	void SetText(float height, float width, float interval, bool italic, COLOR color);
	float m_charH;
	float m_charW;
	float m_charI;
	bool m_italicText;
	COLOR m_foreColor;
	
	void SetBackgrounds(IMAGE mainpanel, IMAGE button, IMAGE textbox, IMAGE listbox);
	IMAGE m_backPanel;
	IMAGE m_backButton;
	IMAGE m_backTextbox;
	IMAGE m_backListbox;
};

#endif