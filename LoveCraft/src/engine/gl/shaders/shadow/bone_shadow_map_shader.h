#ifndef SHADOW_MAP_SHADER_H__
#define	SHADOW_MAP_SHADER_H__

#define MAX_BONES 100

#include "../shader.h"
#include "util/matrix4.h"
#include "util/vector3.h"

class BoneShadowMapShader : public Shader 
{
public:
	BoneShadowMapShader();

	virtual bool Init();

	void SetWVP(const Matrix4f& wvp);
	void SetBoneTransform( uint32 index, const Matrix4f& transform );

	void SetTextureUnit(unsigned int textureUnit);

private:
	GLuint m_WVPLocation;
	GLuint m_boneLocation[MAX_BONES];
	GLint m_textureLocation;
};


#endif
