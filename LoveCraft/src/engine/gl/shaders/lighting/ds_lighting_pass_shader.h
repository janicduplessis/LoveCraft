#ifndef DSLIGHTPASS_H__
#define	DSLIGHTPASS_H__

#include <string>

#include "define.h"

#include "../shader.h"

#include "util/matrix4.h"
#include "util/vector3.h"

class DSLightingPassShader : public Shader {
public:

    DSLightingPassShader();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
    void SetPositionTextureUnit(unsigned int TextureUnit);
    void SetColorTextureUnit(unsigned int TextureUnit);
    void SetNormalTextureUnit(unsigned int TextureUnit);
    void SetEyeWorldPos(const Vector3f& EyeWorldPos);
    void SetMatSpecularIntensity(float Intensity);
    void SetMatSpecularPower(float Power);
    void SetScreenSize(unsigned int Width, unsigned int Height);
    
private:

    GLuint m_WVPLocation;
    GLuint m_posTextureUnitLocation;
    GLuint m_normalTextureUnitLocation;
    GLuint m_colorTextureUnitLocation;
    GLuint m_eyeWorldPosLocation;
    GLuint m_matSpecularIntensityLocation;
    GLuint m_matSpecularPowerLocation;
    GLuint m_screenSizeLocation;
};

#endif