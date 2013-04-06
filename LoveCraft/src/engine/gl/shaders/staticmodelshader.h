#ifndef STATIC_MODEL_SHADER_H__
#define STATIC_MODEL_SHADER_H__

#include "shadernew.h"
#include "lights.h"

/**
 * Shader pour calculer l'éclairage
 */
class StaticModelShader : public ShaderNew
{
public:
	StaticModelShader();
	~StaticModelShader();

	virtual bool Init();

	// Propriétés globales
	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);

private:
	// ----------------------------------------------------------------------
	// Program uniform locations
	GLuint m_WVPLocation;					// World viewport
	GLuint m_worldLocation;					// World position
};

#endif