

// This include
#include "background.h"

/*
*	Background Constructor - Loads the shape of the object and the sprite
*	Parameters - file path of the sprite
*	Return - none
*/
Background::Background(char* filePath)
{
	this->LoadShape();
	this->LoadSprite(filePath);
}

/*
*	Background Destructor
*	Parameters - none
*	Return - none
*/
Background::~Background()
{

}

/*
*	Loads the shape of the opengl object that the sprite will be drawn on
*	Parameters - none
*	Return - void
*/
void Background::LoadShape()
{
	// Set the shape to fill the screen
	std::vector<GLfloat> vertexData = {
		//  X	  Y		  Z			 U     V
		-1.0f,  -1.0f,	1.0f,		1.0f, 0.0f,
		 1.0f,  -1.0f,	1.0f,		0.0f, 0.0f,
		-1.0f,	 1.0f,	1.0f,		1.0f, 1.0f,
		 1.0f,  -1.0f,	1.0f,		0.0f, 0.0f,
		 1.0f,	 1.0f,	1.0f,		0.0f, 1.0f,
		-1.0f,	 1.0f,	1.0f,		1.0f, 1.0f,
	};

	// make and bind the VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// make and bind the VBO
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), &vertexData[0], GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(m_program->attrib("vert"));
	glVertexAttribPointer(m_program->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(m_program->attrib("vertTexCoord"));
	glVertexAttribPointer(m_program->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	// unbind the VAO
	glBindVertexArray(0);
}