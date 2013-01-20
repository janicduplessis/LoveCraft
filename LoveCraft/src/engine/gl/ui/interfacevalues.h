#ifndef INTERFACEVALUES
#define INTERFACEVALUES

#include "define.h"
#include "util/vector2.h"
#include "interfaceinfos.h"
#include "engine/gl/texture.h"
#include "game/character.h"
#include "engine/player.h"


class InterfaceValues
{
public:
	InterfaceValues();
	~InterfaceValues();
	void Init(Player* player, Character* character, Texture** textures, Texture** fonts);
	void Update(float fps, Vector2i mousepos, int width, int height);
	Player* GetPlayer() const;
	Character* GetCharacter() const;
	Texture* GetTexture(unsigned short index) const;
	Texture* GetFont(unsigned short index) const;

	Vector2i GetMousePos() const;
	float GetFPS() const;
	int GetWidth() const;
	int GetHeight() const;
private:
	Player* m_player;
	Character* m_character;
	Texture** m_textures;
	Texture** m_fonts;

	Vector2i m_mousepos;
	float m_fps;
	int m_width;
	int m_height;
};

#endif