

// This include
#include "shader.h"

// Library includes
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

/*
*	Shader constructor - creates the shader
*	Parameters - shader file path, and the type of shader
*	Return - none
*/
Shader::Shader(const std::string& shaderCode, GLenum shaderType) :
	m_Object(0),
	m_uRefCount(NULL)
{
	//create the shader object
	m_Object = glCreateShader(shaderType);
	if (m_Object == 0)
		throw std::runtime_error("glCreateShader failed");

	//set the source code
	const char* code = shaderCode.c_str();
	glShaderSource(m_Object, 1, (const GLchar**)&code, NULL);

	//compile
	glCompileShader(m_Object);

	//throw exception if compile error occurred
	GLint status;
	glGetShaderiv(m_Object, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) 
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(m_Object, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(m_Object, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(m_Object);
		m_Object = 0;
		throw std::runtime_error(msg);
	}

	m_uRefCount = new unsigned;
	*m_uRefCount = 1;
}

/*
*	Shader copy constructor
*	Parameters - the shader to copy to
*	Return - none
*/
Shader::Shader(const Shader& other) :
	m_Object(other.m_Object),
	m_uRefCount(other.m_uRefCount)
{
	Retain();
}

/*
*	Shader Destructor
*	Parameters - none
*	Return - none
*/
Shader::~Shader() 
{
	//m_uRefCount will be NULL if constructor failed and threw an exception
	if (m_uRefCount) Release();
}

/*
*	Returns the object
*	Parameters - none
*	Return - the object
*/
GLuint Shader::GetObject() const 
{
	return m_Object;
}

/*
*	Equals overload operator - copies variables from one shader to the other
*	Parameters - shader to make this equal to
*	Return - reference to a shader
*/
Shader& Shader::operator = (const Shader& other) 
{
	Release();
	m_Object = other.m_Object;
	m_uRefCount = other.m_uRefCount;
	Retain();
	return *this;
}

/*
*	Loads the shader from a file
*	Parameters - the file path of the shader, and the type of shader
*	Return - A shader
*/
Shader Shader::GetShaderFromFile(const std::string& filePath, GLenum shaderType) 
{
	//open file
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) 
	{
		throw std::runtime_error(std::string("Failed to open file: ") + filePath);
	}

	//read whole file into stringstream buffer
	std::stringstream buffer;
	buffer << f.rdbuf();

	//return new shader
	Shader shader(buffer.str(), shaderType);
	return shader;
}

/*
*	Adds to the reference count
*	Parameters - none
*	Return - void
*/
void Shader::Retain() {
	assert(m_uRefCount);
	*m_uRefCount += 1;
}

/*
*	Destroys the shader
*	Parameters - none
*	Return - void
*/
void Shader::Release() {
	assert(m_uRefCount && *m_uRefCount > 0);
	*m_uRefCount -= 1;
	if (*m_uRefCount == 0) 
	{
		glDeleteShader(m_Object); m_Object = 0;
		delete m_uRefCount; m_uRefCount = NULL;
	}
}