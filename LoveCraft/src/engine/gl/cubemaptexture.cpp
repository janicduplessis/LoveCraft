#include "cubemaptexture.h"
#include "util/tool.h"


CubeMapTexture::CubeMapTexture( const std::string& directory, 
							   const std::string& posXFile, 
							   const std::string& negXFile, 
							   const std::string& posYFile, 
							   const std::string& negYFile, 
							   const std::string& posZFile, 
							   const std::string& negZFile )
{
	m_files[0] = directory + posXFile;
	m_files[1] = directory + negXFile;
	m_files[2] = directory + posYFile;
	m_files[3] = directory + negYFile;
	m_files[4] = directory + posZFile;
	m_files[5] = directory + negZFile;
}

CubeMapTexture::~CubeMapTexture()
{

}

bool CubeMapTexture::Load()
{
	CHECK_GL_ERROR();
	glGenTextures(1, &m_texture);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	CHECK_GL_ERROR();
	// Initialize Devil only once:
	static bool alreadyInitialized = false;
	if(!alreadyInitialized)
	{
		ilInit();
		alreadyInitialized = true;
	}
	GLenum types[6];
	types[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	types[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	types[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	types[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	types[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	types[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_files); ++i)
	{
		// Use Devil library to load image data in memory
		ILuint texid;
		ilGenImages(1, &texid);
		ilBindImage(texid);
		if (!ilLoadImage((const ILstring)m_files[i].c_str()))
			return false;

		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			return false;

		glTexImage2D(types[i], 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA,
			GL_UNSIGNED_BYTE, ilGetData());
	}
	CHECK_GL_ERROR();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR();
	return true;
}

void CubeMapTexture::Bind( GLenum textureUnit )
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

