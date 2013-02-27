#include "fontcolor.h"
#include "engine/gl/texture.h"
#include <cassert>

Texture** FontColor::m_textures = 0;

Texture* FontColor::Get(COLOR image)
{
	assert(image >= 0 && image < TEXTCOLOR_LAST);

	if (!m_textures)
	{
		m_textures = new Texture*[TEXTCOLOR_LAST];
		for (uint8 i = 0; i < TEXTCOLOR_LAST; i++)
			m_textures[i] = new Texture();
		InitColor();
	}
	return m_textures[image];
}

void FontColor::InitColor()
{
	m_textures[TEXTCOLOR_WHITE]->Load(TEXTURE_PATH "font.png");
	m_textures[TEXTCOLOR_RED]->Load(TEXTURE_PATH "font_red.png");
	m_textures[TEXTCOLOR_GREEN]->Load(TEXTURE_PATH "font_green.png");
	m_textures[TEXTCOLOR_BLUE]->Load(TEXTURE_PATH "font_blue.png");
	m_textures[TEXTCOLOR_YELLOW]->Load(TEXTURE_PATH "font_yellow.png");
}