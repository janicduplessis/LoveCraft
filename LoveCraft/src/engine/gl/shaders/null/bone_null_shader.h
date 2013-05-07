#ifndef BONE_NULLSHADER_H__
#define	BONE_NULLSHADER_H__

#include "define.h"

#include "../shader.h"
#include "util/matrix4.h"

#define MAX_BONES 100

class BoneNullShader : public Shader 
{
public:
    BoneNullShader();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
	void SetBoneTransform( uint32 index, const Matrix4f& transform );

private:
    GLuint m_WVPLocation;
	GLuint m_boneLocation[MAX_BONES];
};

#endif