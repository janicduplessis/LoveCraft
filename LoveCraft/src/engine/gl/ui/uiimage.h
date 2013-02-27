#ifndef UIIMAGE_H_
#define UIIMAGE_H_

#include "define.h"
#include "interfaceinfos.h"
#include "enumerators.h"

class Texture;

class UIImage
{
public:
	static Texture* Get(IMAGE image);
private:
	static void InitImages();
	static Texture** m_textures;
};

#endif