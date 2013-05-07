#ifndef DSPOINTLIGHTINGPASSSHADER_H
#define	DSPOINTLIGHTINGPASSSHADER_H

#include "ds_lighting_pass_shader.h"
#include "lights.h"

class DSPointLightingPassShader : public DSLightingPassShader
{
public:
    DSPointLightingPassShader();

    bool Init();

    void SetPointLight(const PointLight& Lights);
    
private:
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Position;
        struct {
            GLuint Constant;
            GLuint Linear;
            GLuint Exp;
        } Atten;
    } m_pointLightLocation;   
};


#endif