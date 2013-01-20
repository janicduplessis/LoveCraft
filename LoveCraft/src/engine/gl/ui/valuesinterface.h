#ifndef VALUES_INTERFACE_H__
#define VALUES_INTERFACE_H__

#include "define.h"
#include "util/vector2.h"
#include "engine/gl/texture.h"

struct ValuesInterface
{
	virtual void Init(Texture** _textures, Texture** _fonts) 
	{
		UITextures = _textures;
		FontTextures = _fonts;
	}

	virtual void Update(const Vector2i& _mousepos, int _width, int _height) 
	{
		if (Mousepos != _mousepos)
			Mousepos = _mousepos;
		if (Width != _width)
			Width = _width;
		if (Height != _height)
			Height = _height;
	}

	Texture** UITextures;
	Texture** FontTextures;

	Vector2i Mousepos;

	int Width;
	int Height;
};

#endif