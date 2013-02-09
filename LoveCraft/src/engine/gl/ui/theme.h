#ifndef THEME_H__
#define THEME_H__

#include "define.h"
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
	Texture* GetBackground() const;
private:
	static void InitThemes();
	void Init(string name);
	void Set(float height, float width, float interval, bool italic, Texture* color, Texture* background);
	static Theme** m_themes;
	string m_name;
	float m_charH;
	float m_charW;
	float m_charI;
	bool m_italicText;
	Texture* m_foreColor;
	Texture* m_backColor;
};

#endif