#ifndef SKYBOX_SHADER_H__
#define SKYBOX_SHADER_H__

#include "define.h"
#include "../shader.h"

#include "util/matrix4.h"

class SkyboxShader : public Shader
{
public:
	SkyboxShader();
	~SkyboxShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& wvp);
	void SetTextureUnit(unsigned int textureUnit);

private:
	GLuint m_WVPLocation;
	GLuint m_textureLocation;
};

#endif