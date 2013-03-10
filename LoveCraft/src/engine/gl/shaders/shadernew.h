#ifndef SHADER_NEW_H__
#define SHADER_NEW_H__

#include "define.h"

class ShaderNew
{
public:

	ShaderNew(const char* pEffectFile);

	~ShaderNew();

	void Enable();

protected:

	bool CompileProgram(const char* pProgram);

	GLint GetUniformLocation(const char* pUniformName);

	GLint GetProgramParam(GLint param);

private:    
	GLint m_effect;    
	GLint m_shaderProg;
	const char* m_pEffectFile;
};

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

#endif