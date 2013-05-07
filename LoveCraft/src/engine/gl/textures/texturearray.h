﻿#ifndef TEXTUREARRAY_H__
#define TEXTUREARRAY_H__

#include "define.h"
#include "texture.h"

#include <string>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <IL/ilu.h>
#include <list>

#include <iostream>

/**
 * @brief Classe pour utiliser avec un TextureArray2d de OpenGL
 * 
 * Redimensionne les textures et génère des mipmaps
 */
class TextureArray
{
public:
	/// Index de la texture
	typedef unsigned int TextureIndex;

public:
	/**
	 * Constructeur, les textures seront toutes redimentionnés
	 * avec les dimensions spécifiés dans @textureSize
	 * 
	 * @param textureSize dimension des textures
	 */
	TextureArray(int textureSize);

	/**
	 * Destructeur
	 */
	~TextureArray();

	/**
	 * Ajoute une texture et retourne son index
	 * dans le TextureArray2d de opengl
	 * 
	 * @returns index de la texture ajoutée
	 */
	TextureIndex AddTexture(const std::string& fname);

	/**
	 * Génère le texture array avec les textures ajoutées
	 */
	void Generate();

	/**
	 * Bind la texture
	 */
	void Use(GLenum textureUnit);
	Texture* GetTexture(unsigned int index);
private:
	struct TextureInfo
	{
		ILuint texId;
		std::string texPath;
		TextureIndex index;

		TextureInfo(ILuint _texId, std::string _path, TextureIndex _index) : texId(_texId), texPath(_path), index(_index) {}
	};

	typedef std::list<TextureInfo> TextureList;
	TextureList m_textureList; // Liste avec les path des textures

	TextureIndex m_currentTextureIndex; // Index courant

	int m_textureSize;  // Dimension des textures
	unsigned char* m_data; // Data des textures

	GLuint m_texture; // Id du vbo

	Texture* m_individualTextures;
};

#endif // TEXTUREARRAY_H
