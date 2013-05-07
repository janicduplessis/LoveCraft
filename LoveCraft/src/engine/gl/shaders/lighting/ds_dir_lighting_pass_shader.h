#ifndef DSDIRLIGHTINGPASSSHADER_H__
#define	DSDIRLIGHTINGPASSSHADER_H__

#include "define.h"

#include "ds_lighting_pass_shader.h"
#include "lights.h"

class DSDirLightingPassShader : public DSLightingPassShader 
{
public:
    DSDirLightingPassShader();
    
    virtual bool Init();

    void SetDirectionalLight(const DirectionalLight& Light);

private:
    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } m_dirLightLocation;
};

#endif