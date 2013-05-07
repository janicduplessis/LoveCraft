#ifndef MODEL_SHADER_H__
#define MODEL_SHADER_H__

#include "define.h"

#include "../shader.h"
#include "util/matrix4.h"

/**
 * Shader pour calculer l'éclairage
 */
class BoneModelShader : public Shader
{
public:
	static const uint32 MAX_BONES = 100;

	BoneModelShader();
	~BoneModelShader();

	virtual bool Init();

	// Propriétés globales
	void SetWVP(const Matrix4f& WVP);
	void SetWorld(const Matrix4f& world);

	// Bones
	void SetBoneTransform(uint32 index, const Matrix4f& transform);

private:
	// ----------------------------------------------------------------------
	// Program uniform locations
	GLuint m_WVPLocation;					// World viewport
	GLuint m_worldLocation;					// World position

	GLuint m_boneLocation[MAX_BONES];
};

#endif