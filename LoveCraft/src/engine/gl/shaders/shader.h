#ifndef SHADER_H__
#define SHADER_H__

#include "define.h"

class Shader
{
public:

	Shader(const char* pEffectFile);

	~Shader();

	void Enable();

protected:

	bool CompileProgram(const char* pProgram);

	GLint GetUniformLocation(const char* pUniformName);

	GLint GetProgramParam(GLint param);

protected:    
	GLint m_effect;    
	GLint m_shaderProg;
	const char* m_pEffectFile;
};

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

#endif