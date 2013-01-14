#include "lightingshader.h"

LightingShader::LightingShader()
{

}

LightingShader::~LightingShader()
{

}

bool LightingShader::Init()
{
	Shader::Init();
	AddShader(GL_VERTEX_SHADER, SHADER_PATH "lightingshader.vert");
	AddShader(GL_FRAGMENT_SHADER, SHADER_PATH "lightingshader.frag");
	Link();
	return true;
}

void LightingShader::SetWVP( const Matrix4f& WVP )
{

}

void LightingShader::SetTextureUnit( unsigned int TextureUnit )
{

}

void LightingShader::SetDirectionalLight( const DirectionalLight& Light )
{

}

