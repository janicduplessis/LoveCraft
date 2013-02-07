#include "texture.h"
#include <cassert>

Texture::Texture(const std::string& filename) : m_isValid(false)
{
	if(filename != "")
		Load(filename);
}

Texture::~Texture()
{
	if(IsValid())
		glDeleteTextures(1, &m_textureId);
}


bool Texture::Load(const std::string& filename, bool mipmaps)
{
	// Initialize Devil only once:
	static bool alreadyInitialized = false;
	if(!alreadyInitialized)
	{
		ilInit();
		alreadyInitialized = true;
	}

	// Use Devil library to load image data in memory
	ILuint texid;
	ilGenImages(1, &texid);
	ilBindImage(texid);
	if (!ilLoadImage((const ILstring)filename.c_str()))
		return false;

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		return false;

	// Create mipmapped opengl texture from image data
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	m_dimensions = Vector2i(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));

	if (mipmaps) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	m_isValid = true;
	return true;
}

bool Texture::IsValid() const
{
	return m_isValid;
}

void Texture::Bind(GLenum tex) const
{
	assert(IsValid());
	glActiveTexture(tex);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

float Texture::GetWidth()
{
	return m_dimensions.x;
}

float Texture::GetHeight()
{
	return m_dimensions.y;
}

