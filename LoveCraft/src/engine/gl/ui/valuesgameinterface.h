#ifndef VALUES_GAME_INTERFACE_H__
#define VALUES_GAME_INTERFACE_H__

#include "valuesinterface.h"

#include "engine/player.h"
#include "game/character.h"
#include "engine/gl/texturearray.h"

struct ValuesGameInterface : ValuesInterface
{
	virtual void Init(Texture** _textures, Texture** _fonts, TextureArray* _blocs, Player* _player, Character* _char)
	{
		ValuesInterface::Init(_textures, _fonts);
		Player = _player;
		Character = _char;
		BlocTextures = _blocs;
	}

	virtual void Update(const Point& _mousepos, int _width, int _height, BlockType _curBlockType, float _fps)
	{
		ValuesInterface::Update(_mousepos, _width, _height);
		if (Fps != _fps)
			Fps = _fps;
		if (CurrentBlocType != _curBlockType)
			CurrentBlocType = _curBlockType;
	}

	Player* Player;
	Character* Character;
	TextureArray* BlocTextures;
	BlockType CurrentBlocType;

	float Fps;
};

#endif // !GAME_INTERFACE_VALUES_H__
