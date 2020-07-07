

// This include
#include "gameobject.h"

/*
*	GameObject Constructor - loads the shaders and creates the program
*	Parameters - none
*	Return - none
*/
GameObject::GameObject() : 
	m_type(OTHER)
{
	// Load shaders
	std::vector<Shader> shaders;
	shaders.push_back(Shader::GetShaderFromFile("Assets/Shaders/vertex-shader.vs", GL_VERTEX_SHADER));
	shaders.push_back(Shader::GetShaderFromFile("Assets/Shaders/fragment-shader.fs", GL_FRAGMENT_SHADER));
	m_program = new Program(shaders);
}

/*
*	GameObject Constructor - loads the shaders and creates the program, Loads the shape and the sprite
*	Parameters - position x and y, width and height, and file path of the sprite
*	Return - none
*/
GameObject::GameObject(float posX, float posY, float width, float height, char* filePath) :
	m_position(b2Vec2(posX, posY)),
	m_width(width),
	m_height(height),
	m_type(OTHER)
{
	// Load shaders
	std::vector<Shader> shaders;
	shaders.push_back(Shader::GetShaderFromFile("Assets\\Shaders\\vertex-shader.vs", GL_VERTEX_SHADER));
	shaders.push_back(Shader::GetShaderFromFile("Assets\\Shaders\\fragment-shader.fs", GL_FRAGMENT_SHADER));
	m_program = new Program(shaders);

	// Load the shape
	LoadShape();

	// Load the texture for the sprite
	LoadSprite(filePath);
}

/*
*	GameObject Destructor - deletes the program
*	Parameters - none
*	Return - none
*/
GameObject::~GameObject()
{
	// Delete pointers
	delete m_program;
	m_program = 0;
}

/*
*	Loads the opengl shape on which the sprite will be drawn
*	Parameters - none
*	Return - void
*/
void GameObject::LoadShape()
{
	float width = m_width / 2.0f * METERSTOUNITS;
	float height = m_height / 2.0f * METERSTOUNITS;

	std::vector<GLfloat> vertexData = {
		//  X		Y		  Z			 U     V
		-width,  -height,	1.0f,		1.0f, 0.0f,
		 width,  -height,	1.0f,		0.0f, 0.0f,
		-width,	  height,	1.0f,		1.0f, 1.0f,
		 width,  -height,	1.0f,		0.0f, 0.0f,
		 width,	  height,	1.0f,		0.0f, 1.0f,
		-width,	  height,	1.0f,		1.0f, 1.0f,
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

/*
*	Load the sprite using SOIL
*	Parameters - file path of the sprite
*	Return - void
*/
void GameObject::LoadSprite(char* path)
{
	// Generate texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;

	// Load actual image
	unsigned char* image = SOIL_load_image(path,
		&width,
		&height,
		0,
		SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image);

	// Bind texture
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*
*	empty update function
*	Parameters - current game time
*	Return - void
*/
void GameObject::Update(float time)
{
	
}

/*
*	Render - draws the sprite with the correct position
*	Parameters - none
*	Return - void
*/
void GameObject::Render()
{
	// bind the program
	m_program->Use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind the texture and set the "tex" uniform in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	m_program->setUniform("tex", 0);
	m_program->setUniform("model", glm::translate(glm::mat4(), glm::vec3(m_position.x * METERSTOUNITS, m_position.y * METERSTOUNITS, 0.0f)));

	// bind the texture and set the "tex" uniform in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	// bind the VAO and draw
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

	// unbind the VAO, the program and the texture
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
	m_program->StopUsing();
}

/*
*	Returns half of the width of the object
*	Parameters - none
*	Return - half-width float
*/
float GameObject::GetHalfWidth()
{
	return m_width / 2.0f;
}

/*
*	Returns half of the height of the object
*	Parameters - none
*	Return - half-height float
*/
float GameObject::GetHalfHeight()
{
	return m_height / 2.0f;
}

/*
*	returns the type of game object that the object is
*	Parameters - none
*	Return - GameObjectType enumerator
*/
GameObjectType GameObject::GetType()
{
	return m_type;
}