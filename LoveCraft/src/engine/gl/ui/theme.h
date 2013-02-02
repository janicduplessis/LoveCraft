#ifndef THEME_H__
#define THEME_H__

#include "define.h"
#include "../texture.h"
#include "util/vector2.h"
#include <cassert>
#include <string>
#include <iostream>

class Theme
{
public:
	Theme();
	~Theme();
	void Init();
private:
	float m_charH;
	float m_charW;
	float m_charI;
	bool m_italicText;
	Texture* m_fontColor;
	Texture* m_background;

};

#endif