#ifndef LIGHTING_SHADER_H__
#define LIGHTING_SHADER_H__

#include "shader.h"

#include <util/vector3.h>
#include <util/matrix4.h>

struct DirectionalLight
{
	Vector3f Color;
	float AmbientIntensity;
	Vector3f Direction;
	float DiffuseIntensity;
};

class LightingShader : public Shader
{
public:
	LightingShader();
	~LightingShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);
	void SetTextureUnit(unsigned int textureUnit);
	void SetDirectionalLight(const DirectionalLight& light);
	void SetEyeWorldPos(const Vector3f& eyePos);
	void SetMatSpecualarIntensity(float intensity);
	void SetMatSpecularPower(float power);

private:
	// ----------------------------------------------------------------------
	// Program uniform locations
	GLuint m_WVPLocation;					// World viewport
	GLuint m_worldLocation;					// World position
	GLuint m_samplerLocation;				// Texture sampler
	GLuint m_eyeWorldPosLocation;			// Camera position
	GLuint m_matSpecularIntensityLocation;	// Material specular intensity
	GLuint m_matSpecularPowerLocation;		// Material specular power

	// Light struct
	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint Direction;
		GLuint DiffuseIntensity;
	} m_dirLightLocation;
};

#endif