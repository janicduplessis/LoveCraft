#ifndef SHADER_H
#define SHADER_H
#include <string>
#include "define.h"
#include <list>

class Shader
{
public:    
	Shader();
	~Shader();

	bool Load(const std::string& vertFile, const std::string& fragFile, bool verbose = false);
	void Use() const;

	bool Init();
	bool AddShader(GLenum shaderType, const std::string& file, bool verbose = false);
	bool Link();

	GLuint BindUniform(const std::string& name) const;
	void UpdateIntUniform(GLint name, GLint value) const;
	void UpdateFloatUniform(GLint name, GLfloat value) const;

	GLenum& GetProgram();

	static void Disable();

protected:
	bool CheckShaderError(GLenum shader, bool verbose);
	bool CheckProgramError(GLenum program, bool showWarning, bool verbose);
	std::string ShaderType2ShaderName(GLuint Type);

protected:
	GLenum m_program;

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};



#endif // SHADER_H
