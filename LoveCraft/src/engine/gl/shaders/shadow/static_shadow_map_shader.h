#ifndef STATIC_SHADOW_MAP_SHADER_H__
#define	STATIC_SHADOW_MAP_SHADER_H__

#include "../shader.h"
#include "util/matrix4.h"
#include "util/vector3.h"

class StaticShadowMapShader : public Shader 
{
public:
	StaticShadowMapShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& wvp);

	void SetTextureUnit(unsigned int textureUnit);

private:
	GLuint m_WVPLocation;
	GLint m_textureLocation;
};


#endif
