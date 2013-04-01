#ifndef FONTCOLOR_H_
#define FONTCOLOR_H_

#include "define.h"
#include "interfaceinfos.h"
#include "enumerators.h"

class Texture;

class FontColor
{
public:
	static Texture* Get(COLOR image);
private:
	static void InitColor();
	static Texture** m_textures;
};

#endif