#ifndef STATIC_SHADOWVOLUMESHADER_H__
#define	STATIC_SHADOWVOLUMESHADER_H__

#define MAX_BONES 100

#include "../shader.h"
#include "util/matrix4.h"
#include "util/vector3.h"

class StaticShadowVolumeShader : public Shader 
{
public:
    StaticShadowVolumeShader();

    virtual bool Init();

    void SetVP(const Matrix4f& VP);
    void SetWorldMatrix(const Matrix4f& World);
    void SetLightPos(const Vector3f& Pos);

private:
    GLuint m_VPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_lightPosLocation;
};


#endif
