#ifndef TEXTUREARRAY_H
#define TEXTUREARRAY_H
#include <string>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <IL/ilu.h>
#include <map>
#include "define.h"

#include <iostream>


class TextureArray
{
public:
	typedef unsigned int TextureIndex;

public:

	TextureArray(int textureSize);

	~TextureArray();

	bool Load(int slot, const std::string& file);

	void Init();

	void Use();

	TextureIndex AddTexture(const std::string& fname);

private:
	struct TextureInfo
	{
		ILuint texId;
		TextureIndex texIdx;

		TextureInfo(ILuint _texId, unsigned int _texIdx) : texId(_texId), texIdx(_texIdx) {}
	};

	typedef std::map<std::string, TextureInfo> TextureList;
	TextureList m_textureList;

	TextureIndex m_currentTextureIndex;

	int m_textureSize;
	unsigned char* m_data;

	GLuint m_texture;
};

#endif // TEXTUREARRAY_H
