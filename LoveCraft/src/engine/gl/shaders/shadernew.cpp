#include "shadernew.h"

#include <string>
#include <iostream>
#include <assert.h>
#include <glfx.h>


#include "util/tool.h"

ShaderNew::ShaderNew(const char* pEffectFile)
{
	m_pEffectFile = pEffectFile;
	m_shaderProg = 0;
	m_effect = glfxGenEffect();
}


ShaderNew::~ShaderNew()
{
	if (m_shaderProg != 0)
	{
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}

	glfxDeleteEffect(m_effect);
}

bool ShaderNew::CompileProgram(const char* pProgram)
{
	if (!glfxParseEffectFromFile(m_effect, m_pEffectFile)) {
		std::string log = glfxGetEffectLog(m_effect);
		std::cout << "Error creating effect from file '" << m_pEffectFile << "':" << std::endl;
		std::cout << log.c_str() << std::endl;
		return false;
	}

	m_shaderProg = glfxCompileProgram(m_effect, pProgram);

	if (m_shaderProg < 0) {
		std::string log = glfxGetEffectLog(m_effect);
		std::cout << "Error compiling program '" << pProgram << "' in effect file '" << m_pEffectFile << "':" << std::endl;
		std::cout << log.c_str() << std::endl;
		return false;
	}

	return true;
}

void ShaderNew::Enable()
{
	glUseProgram(m_shaderProg);
}


GLint ShaderNew::GetUniformLocation(const char* pUniformName)
{
	GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == INVALID_OGL_VALUE) {
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

GLint ShaderNew::GetProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(m_shaderProg, param, &ret);
	return ret;
}