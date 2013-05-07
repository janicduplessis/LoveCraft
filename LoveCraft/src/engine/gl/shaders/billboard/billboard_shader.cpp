#include "billboard_shader.h"
#include "engine/info.h"

static const char* pEffectFile = SHADER_PATH "billboard/billboard.glsl";

BillboardShader::BillboardShader() : Shader(pEffectFile)
{

}

BillboardShader::~BillboardShader()
{

}

bool BillboardShader::Init()
{
	if (!CompileProgram("Billboard")) {
		return false;
	}
	 
	m_VPLocation = GetUniformLocation("gVP");
	m_cameraPosLocation = GetUniformLocation("gCameraPos");
	m_sizeLocation = GetUniformLocation("gBillboardSize");
	m_colorTextureUnitLocation = GetUniformLocation("gColorMap");
	m_colorLocation = GetUniformLocation("gColor");

	return true;
}

void BillboardShader::SetCameraPosition( const Vector3f& pos )
{
	glUniform3f(m_cameraPosLocation, pos.x, pos.y, pos.z);
}

void BillboardShader::SetVP( const Matrix4f& vp )
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)vp.GetInternalValues());
}

void BillboardShader::SetBillboardSize( float size )
{
	glUniform1f(m_sizeLocation, size);
}

void BillboardShader::SetColorTextureUnit( int unit )
{
	glUniform1i(m_colorTextureUnitLocation, unit);
}

void BillboardShader::SetColor( const Vector3f& color )
{
	glUniform3f(m_colorLocation, color.x, color.y, color.z);
}
