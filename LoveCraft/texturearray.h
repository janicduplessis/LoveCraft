#ifndef TEXTUREARRAY_H__
#define TEXTUREARRAY_H__
#include <string>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <IL/ilu.h>
#include <map>
#include "define.h"

#include <iostream>

/**
 * @brief Classe pour utiliser avec un TextureArray2d de OpenGL
 * 
 * Redimensionne les textures et g�n�re des mipmaps
 */
class TextureArray
{
public:
	/// Index de la texture
	typedef unsigned int TextureIndex;

public:
	/**
	 * Constructeur, les textures seront toutes redimentionn�s
	 * avec les dimensions sp�cifi�s dans @textureSize
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
	 * @returns index de la texture ajout�e
	 */
	TextureIndex AddTexture(const std::string& fname);

	/**
	 * G�n�re le texture array avec les textures ajout�es
	 */
	void Generate();

	/**
	 * Bind la texture
	 */
	void Use();

private:
	struct TextureInfo
	{
		ILuint texId;
		TextureIndex texIdx;

		TextureInfo(ILuint _texId, unsigned int _texIdx) : texId(_texId), texIdx(_texIdx) {}
	};

	typedef std::map<std::string, TextureInfo> TextureList;
	TextureList m_textureList; // Liste avec les path des textures

	TextureIndex m_currentTextureIndex; // Index courant

	int m_textureSize;  // Dimension des textures
	unsigned char* m_data; // Data des textures

	GLuint m_texture; // Id du vbo
};

#endif // TEXTUREARRAY_H
