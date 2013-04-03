#include "billboardshader.h"
#include "../../info.h"


BillboardShader::~BillboardShader()
{

}

BillboardShader::BillboardShader()
{

}

bool BillboardShader::Init()
{
	std::cout << "Billboard Shader :" << std::endl;
	Shader::Init();
	AddShader(GL_VERTEX_SHADER_ARB, SHADER_PATH "billboardshader.vert", true);
	AddShader(GL_GEOMETRY_SHADER_ARB, SHADER_PATH "billboardshader.geom", true);
	AddShader(GL_FRAGMENT_SHADER_ARB, SHADER_PATH "billboardshader.frag", true);
	Link();
	 

	m_VPLocation = BindUniform("gVP");
	m_cameraPosLocation = BindUniform("gCameraPos");
	m_sizeLocation = BindUniform("gBillboardSize");
	m_colorTextureUnitLocation = BindUniform("gColorMap");
	m_colorLocation = BindUniform("gColor");

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
