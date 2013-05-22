#ifndef STATIC_NULLSHADER_H__
#define	STATIC_NULLSHADER_H__

#include "define.h"

#include "../shader.h"
#include "util/matrix4.h"

class StaticNullShader : public Shader {
public:

    StaticNullShader();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);

private:

    GLuint m_WVPLocation;
};

#endif