

// Local include
#include "program.h"

// Library inludes
#include <stdexcept>

// Third-party includes
#include "Dependencies\glm\gtc\type_ptr.hpp"

/*
*	Program Constructor - creates shaders
*	Parameters - reference to a vector of shaders
*	Return - none
*/
Program::Program(const std::vector<Shader>& shaders) :
	m_Object(0)
{
	if (shaders.size() <= 0)
		throw std::runtime_error("No shaders were provided to create the program");

	//create the program object
	m_Object = glCreateProgram();
	if (m_Object == 0)
		throw std::runtime_error("glCreateProgram failed");

	//attach all the shaders
	for (unsigned i = 0; i < shaders.size(); ++i)
		glAttachShader(m_Object, shaders[i].GetObject());

	//link the shaders together
	glLinkProgram(m_Object);

	//detach all the shaders
	for (unsigned i = 0; i < shaders.size(); ++i)
		glDetachShader(m_Object, shaders[i].GetObject());

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(m_Object, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(m_Object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(m_Object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(m_Object); m_Object = 0;
		throw std::runtime_error(msg);
	}
}

/*
*	Program Destructor - destroys the program
*	Parameters - none
*	Return - none
*/
Program::~Program()
{
	//might be 0 if ctor fails by throwing exception
	if (m_Object != 0) glDeleteProgram(m_Object);
}

/*
*	returns the program object
*	Parameters - none
*	Return - the program GLuint
*/
GLuint Program::Object() const
{
	return m_Object;
}

/*
*	starts using the program
*	Parameters - none
*	Return - void
*/
void Program::Use() const 
{
	glUseProgram(m_Object);
}

/*
*	Returns whether the is in use or not
*	Parameters - none
*	Return - bool
*/
bool Program::IsInUse() const 
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return (currentProgram == (GLint)m_Object);
}

/*
*	Stops using the program
*	Parameters - none
*	Return - void
*/
void Program::StopUsing() const 
{
	assert(IsInUse());
	glUseProgram(0);
}

/*
*	returns a given attrib
*	Parameters - attrib name
*	Return - the attribute
*/
GLint Program::attrib(const GLchar* attribName) const 
{
	if (!attribName)
		throw std::runtime_error("attribName was NULL");

	GLint attrib = glGetAttribLocation(m_Object, attribName);
	if (attrib == -1)
		throw std::runtime_error(std::string("Program attribute not found: ") + attribName);

	return attrib;
}

/*
*	returns a given uniform
*	Parameters - uniform name
*	Return - the uniform
*/
GLint Program::uniform(const GLchar* uniformName) const 
{
	if (!uniformName)
		throw std::runtime_error("uniformName was NULL");

	GLint uniform = glGetUniformLocation(m_Object, uniformName);
	if (uniform == -1)
		throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);

	return uniform;
}

#define ATTRIB_N_UNIFORM_SETTERS(OGL_TYPE, TYPE_PREFIX, TYPE_SUFFIX) \
\
void Program::setAttrib(const GLchar* name, OGL_TYPE v0) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX (attrib(name), v0); } \
void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX (attrib(name), v0, v1); } \
void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX (attrib(name), v0, v1, v2); } \
void Program::setAttrib(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX (attrib(name), v0, v1, v2, v3); } \
\
void Program::setAttrib1v(const GLchar* name, const OGL_TYPE* v) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 1 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void Program::setAttrib2v(const GLchar* name, const OGL_TYPE* v) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 2 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void Program::setAttrib3v(const GLchar* name, const OGL_TYPE* v) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 3 ## TYPE_SUFFIX ## v (attrib(name), v); } \
void Program::setAttrib4v(const GLchar* name, const OGL_TYPE* v) \
    { assert(IsInUse()); glVertexAttrib ## TYPE_PREFIX ## 4 ## TYPE_SUFFIX ## v (attrib(name), v); } \
\
void Program::setUniform(const GLchar* name, OGL_TYPE v0) \
    { assert(IsInUse()); glUniform1 ## TYPE_SUFFIX (uniform(name), v0); } \
void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1) \
    { assert(IsInUse()); glUniform2 ## TYPE_SUFFIX (uniform(name), v0, v1); } \
void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2) \
    { assert(IsInUse()); glUniform3 ## TYPE_SUFFIX (uniform(name), v0, v1, v2); } \
void Program::setUniform(const GLchar* name, OGL_TYPE v0, OGL_TYPE v1, OGL_TYPE v2, OGL_TYPE v3) \
    { assert(IsInUse()); glUniform4 ## TYPE_SUFFIX (uniform(name), v0, v1, v2, v3); } \
\
void Program::setUniform1v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
    { assert(IsInUse()); glUniform1 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void Program::setUniform2v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
    { assert(IsInUse()); glUniform2 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void Program::setUniform3v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
    { assert(IsInUse()); glUniform3 ## TYPE_SUFFIX ## v (uniform(name), count, v); } \
void Program::setUniform4v(const GLchar* name, const OGL_TYPE* v, GLsizei count) \
    { assert(IsInUse()); glUniform4 ## TYPE_SUFFIX ## v (uniform(name), count, v); }

ATTRIB_N_UNIFORM_SETTERS(GLfloat, , f);
ATTRIB_N_UNIFORM_SETTERS(GLdouble, , d);
ATTRIB_N_UNIFORM_SETTERS(GLint, I, i);
ATTRIB_N_UNIFORM_SETTERS(GLuint, I, ui);

void Program::setUniformMatrix2(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix2fv(uniform(name), count, transpose, v);
}

void Program::setUniformMatrix3(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix3fv(uniform(name), count, transpose, v);
}

void Program::setUniformMatrix4(const GLchar* name, const GLfloat* v, GLsizei count, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix4fv(uniform(name), count, transpose, v);
}

void Program::setUniform(const GLchar* name, const glm::mat2& m, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix2fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(const GLchar* name, const glm::mat3& m, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix3fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(const GLchar* name, const glm::mat4& m, GLboolean transpose) 
{
	assert(IsInUse());
	glUniformMatrix4fv(uniform(name), 1, transpose, glm::value_ptr(m));
}

void Program::setUniform(const GLchar* uniformName, const glm::vec3& v) 
{
	setUniform3v(uniformName, glm::value_ptr(v));
}

void Program::setUniform(const GLchar* uniformName, const glm::vec4& v) 
{
	setUniform4v(uniformName, glm::value_ptr(v));
}