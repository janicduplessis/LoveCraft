#ifndef SHADOWVOLUMESHADER_H__
#define	SHADOWVOLUMESHADER_H__

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
    
private:
    GLuint m_VPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_lightPosLocation;
};


#endif
