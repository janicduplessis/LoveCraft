#include "shader.h"
#include "define.h"
#include "util/tool.h"
#include <iostream>
#include <cassert>
#include "../info.h"

#ifndef WINDOWS

std::string Shader::ShaderType2ShaderName(GLuint Type)
{
	switch (Type) {
	case GL_VERTEX_SHADER:
		return "Vertex";
	case GL_GEOMETRY_SHADER:
		return "Geometry";
	case GL_FRAGMENT_SHADER:
		return "Fragment";
	default:
		assert(0);
	}

	return NULL;
}

bool Shader::Load(const std::string& vertFile, const std::string& fragFile, bool verbose)
{
	Init();
	AddShader(GL_VERTEX_SHADER_ARB, vertFile, verbose);
	AddShader(GL_FRAGMENT_SHADER_ARB, fragFile, verbose);
	Link();

	return true;
}

bool Shader::Init()
{
	m_program = glCreateProgram();
	return true;
}

bool Shader::AddShader(GLenum shaderType, const std::string& file, bool verbose) 
{
	std::string shaderText;

	if(!Tool::LoadTextFile(file, shaderText))
	{
		if(verbose)
			std::cout << "Failed to load " << file << std::endl;
		return false;
	}

	const char * my_shader_source = shaderText.c_str();

	GLuint shader = glCreateShader(shaderType);
	CHECK_GL_ERROR();
	assert(glIsShader(shader));

	m_shaderObjList.push_back(shader);

	glShaderSource(shader, 1, (const GLchar**)&my_shader_source, NULL);
	CHECK_GL_ERROR();

	if(verbose)
		std::cout << "Compiling " << ShaderType2ShaderName(shaderType) << " shader (" << file << ")..." << std::endl;
	glCompileShader(shader);
	if(!CheckShaderError(shader, verbose))
		return false;

	glAttachShader(m_program, shader);
	CHECK_GL_ERROR();
}

bool Shader::Link() {
	glLinkProgram(m_program);

	// Delete the intermediate shader objects that have been added to the program
	for (ShaderObjList::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
	{
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return true;
}

void Shader::Use() const
{
    glUseProgram(m_program);
}

GLuint Shader::BindUniform(const std::string& name) const
{
    return glGetUniformLocation(m_program, name.c_str());
}

void Shader::UpdateIntUniform(GLint name, GLint value) const
{
    glUniform1i(name, value);
}

void Shader::UpdateFloatUniform(GLint name, GLfloat value) const
{
    glUniform1f(name, value);
}

void Shader::Disable()
{
    glUseProgram(0);
}

bool Shader::CheckShaderError(GLenum shader, bool verbose)
{
    GLint compileOk;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk);
    if(verbose && !compileOk)
    {
        int maxLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog);

        std::cout << infoLog << std::endl;
        delete [] infoLog;
        return false;
    }

    return compileOk;
}

bool Shader::CheckProgramError(GLenum program, bool showWarning, bool verbose)
{
    GLint compileOk;

    glGetProgramiv(program, GL_LINK_STATUS, &compileOk);
    CHECK_GL_ERROR();
    if(verbose && (showWarning || !compileOk))
    {
        int maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        CHECK_GL_ERROR();

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);
        CHECK_GL_ERROR();

        std::cout << infoLog << std::endl;
        delete [] infoLog;
    }

    return compileOk;
}

GLenum& Shader::GetProgram()
{
	return m_program;
}




#endif
