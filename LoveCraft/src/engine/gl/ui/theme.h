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
	Texture* GetForeColor() const;

	Texture* GetBackBackPanel() const;
	Texture* GetBackMainPanel() const;
	Texture* GetBackButton() const;
	Texture* GetBackTextbox() const;
	Texture* GetBackListbox() const;
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
	Texture* m_foreColor;
	
	void SetBackgrounds(IMAGE mainpanel, IMAGE button, IMAGE textbox, IMAGE listbox);
	Texture* m_backPanel;
	Texture* m_backButton;
	Texture* m_backTextbox;
	Texture* m_backListbox;
};

#endif