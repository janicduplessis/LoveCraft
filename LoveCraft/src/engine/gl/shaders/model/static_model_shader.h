#ifndef STATIC_MODEL_SHADER_H__
#define STATIC_MODEL_SHADER_H__

#include "define.h"

#include "../shader.h"
#include "util/matrix4.h"

/**
 * Shader render un modele static
 */
class StaticModelShader : public Shader
{
public:
	StaticModelShader();
	~StaticModelShader();

	virtual bool Init();

	// Propriétés globales
	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);
	void SetColorTextureUnit(unsigned int unit);
	void SetNormalTextureUnit(unsigned int unit);

private:
	// ----------------------------------------------------------------------
	// Program uniform locations
	GLuint m_WVPLocation;					// World viewport
	GLuint m_worldLocation;					// World position
	GLuint m_colorSamplerLocation;
	GLuint m_normalSamplerLocation;
};

#endif