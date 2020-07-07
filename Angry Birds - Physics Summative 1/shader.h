

#pragma once

#ifndef SHADER_H
#define SHADER_H

// Third-party includes
#include "Dependencies\glew\glew.h"

// Library includes
#include <string>

class Shader
{
public:	

	// Constructor, creates a shader from a string of shader source code
	Shader(const std::string& shaderCode, GLenum shaderType);

	// Destructor
	~Shader();

	// Copy constructor and = operator overload
	Shader(const Shader& other);
	Shader& operator =(const Shader& other);

	// Creates a shader from a text file
	static Shader GetShaderFromFile(const std::string& filePath, GLenum shaderType);

	// Returns the shaders' object ID
	GLuint GetObject() const;
	
private:

	GLuint m_Object;
	unsigned* m_uRefCount;

	void Retain();
	void Release();
};

#endif