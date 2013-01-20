#include "interfacevalues.h"


InterfaceValues::InterfaceValues() : m_player(0), m_character(0), m_textures(0), m_fonts(0),
	m_mousepos(Vector2i()), m_fps(0)
{
}


InterfaceValues::~InterfaceValues()
{
}

void InterfaceValues::Init(Player* player, Character* character, Texture** textures, Texture** fonts)
{
	m_player = player;
	m_character = character;
	m_textures = textures;
	m_fonts = fonts;
}

void InterfaceValues::Update(float fps, Vector2i mousepos, int width, int height)
{
	if (fps != m_fps)
		m_fps = fps;
	if (mousepos != m_mousepos)
		m_mousepos = mousepos;
	if (width != m_width)
		m_width = width;
	if (height != m_height)
		m_height = height;
}

Player* InterfaceValues::GetPlayer() const
{
	return m_player;
}
Character* InterfaceValues::GetCharacter() const
{
	return m_character;
}
Texture* InterfaceValues::GetTexture(unsigned short index) const
{
	return m_textures[index];
}
Texture* InterfaceValues::GetFont(unsigned short index) const
{
	return m_fonts[index];
}
Vector2i InterfaceValues::GetMousePos() const
{
	return m_mousepos;
}
float InterfaceValues::GetFPS() const
{
	return m_fps;
}
int InterfaceValues::GetWidth() const
{
	return m_width;
}
int InterfaceValues::GetHeight() const
{
	return m_height;
}