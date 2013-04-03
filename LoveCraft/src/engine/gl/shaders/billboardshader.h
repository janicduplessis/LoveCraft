#ifndef BILLBOARD_SHADER_H__
#define BILLBOARD_SHADER_H__

#include "shader.h"
#include "define.h"

#include <util/matrix4.h>

class BillboardShader : public Shader
{
public:
	BillboardShader();
	~BillboardShader();

	bool Init();

	void SetVP(const Matrix4f& vp);
	void SetCameraPosition(const Vector3f& pos);
	void SetBillboardSize(float size);
	void SetColorTextureUnit(int unit);
	void SetColor(const Vector3f& color);

private:
	GLuint m_VPLocation;
	GLuint m_cameraPosLocation;
	GLuint m_sizeLocation;
	GLuint m_colorTextureUnitLocation;
	GLuint m_colorLocation;
};

#endif