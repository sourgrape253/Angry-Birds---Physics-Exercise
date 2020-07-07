

#pragma once

#ifndef PROGRAM_H
#define PROGRAM_H

// Local includes
#include "shader.h"

// Library includes
#include <vector>

// Third-party includes
#include "Dependencies\glm\glm.hpp"

class Program
{
public:

	// Constructor, creates a program by linking a list of shader objects
	Program(const std::vector<Shader>& shaders);

	// Destructor
	~Program();

	// Returns the instance of the program
	GLuint Object() const;

	void Use() const;
	bool IsInUse() const;
	void StopUsing() const;

	// Returns attribute/uniform index for the given name
	GLint attrib(const GLchar* attribName) const;
	GLint uniform(const GLchar* uniformName) const;

	// Setters for uniform and attribute objects
#define PROGRAM_ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE) \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setAttrib(const GLchar* attribName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
    void setAttrib1v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib2v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib3v(const GLchar* attribName, const OGL_TYPE* v); \
    void setAttrib4v(const GLchar* attribName, const OGL_TYPE* v); \
\
    void setUniform(const GLchar* uniformName, OGL_TYPE v0); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2); \
    void setUniform(const GLchar* uniformName, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3); \
\
    void setUniform1v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform2v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform3v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \
    void setUniform4v(const GLchar* uniformName, const OGL_TYPE* v, GLsizei count=1); \

	PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLfloat)
	PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLdouble)
	PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLint)
	PROGRAM_ATTRIB_N_UNIFORM_SETTERS(GLuint)

	void setUniformMatrix2(const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniformMatrix3(const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniformMatrix4(const GLchar* uniformName, const GLfloat* v, GLsizei count = 1, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* uniformName, const glm::mat2& m, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* uniformName, const glm::mat3& m, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* uniformName, const glm::mat4& m, GLboolean transpose = GL_FALSE);
	void setUniform(const GLchar* uniformName, const glm::vec3& v);
	void setUniform(const GLchar* uniformName, const glm::vec4& v);

private:

	GLuint m_Object;

	// Functions are private to prevent copying
	Program(const Program&);
	const Program& operator=(const Program&);
};

#endif