#ifndef SHADOW_MAP_FBO_H__
#define SHADOW_MAP_FBO_H__

#include "define.h"

class ShadowMapFBO
{
public:
	ShadowMapFBO();

	~ShadowMapFBO();

	bool Init(unsigned int windowWidth, unsigned int windowHeight);

	void BindForWriting();

	void BindForReading(GLenum textureUnit);

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
};

#endif