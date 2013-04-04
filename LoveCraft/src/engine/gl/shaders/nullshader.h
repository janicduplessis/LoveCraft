#ifndef NULLSHADER_H__
#define	NULLSHADER_H__

#include "define.h"

#include "shadernew.h"
#include "util/matrix4.h"

class NullShader : public ShaderNew {
public:

    NullShader();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);

private:

    GLuint m_WVPLocation;
};

#endif