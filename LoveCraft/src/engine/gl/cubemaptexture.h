#ifndef CUBEMAPTEXTURE_H__
#define CUBEMAPTEXTURE_H__

#include "define.h"
#include <string>

#include <IL/il.h>

class CubeMapTexture
{
public:
	CubeMapTexture(const std::string& directory,
		const std::string& posXFile,
		const std::string& negXFile,
		const std::string& posYFile,
		const std::string& negYFile,
		const std::string& posZFile,
		const std::string& negZFile);

	~CubeMapTexture();

	bool Load();

	void Bind(GLenum textureUnit);

private:
	std::string m_files[6];
	GLuint m_texture;
};

#endif // !CUBEMAPTEXTURE_H__
