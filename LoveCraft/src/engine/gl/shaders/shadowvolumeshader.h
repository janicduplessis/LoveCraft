#ifndef SHADOWVOLUMESHADER_H__
#define	SHADOWVOLUMESHADER_H__

#define MAX_BONES 100

#include "shadernew.h"
#include "util/matrix4.h"
#include "util/vector3.h"

class ShadowVolumeShader : public ShaderNew 
{
public:
    ShadowVolumeShader();

    virtual bool Init();

    void SetVP(const Matrix4f& VP);
    void SetWorldMatrix(const Matrix4f& World);
    void SetLightPos(const Vector3f& Pos);
	void SetBoneTransform( uint32 index, const Matrix4f& transform );

private:
    GLuint m_VPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_lightPosLocation;
	GLuint m_boneLocation[MAX_BONES];
};


#endif
