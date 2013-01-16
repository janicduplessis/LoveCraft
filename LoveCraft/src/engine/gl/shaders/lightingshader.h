#ifndef LIGHTING_SHADER_H__
#define LIGHTING_SHADER_H__

#include "shader.h"

#include <util/vector3.h>
#include <util/matrix4.h>

struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;
};

class LightingShader : public Shader
{
public:
	LightingShader();
	~LightingShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);

private:
	GLuint m_WVPLocation;
	GLuint m_samplerLocation;
	GLuint m_dirLightColorLocation;
	GLuint m_dirLightAmbientIntensityLocation;
};

#endif