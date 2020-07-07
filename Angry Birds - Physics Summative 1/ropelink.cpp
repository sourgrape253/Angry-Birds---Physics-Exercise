

//This include
#include "ropelink.h"

/*
*	Ropelink Constructor - Sets variables, loads the sprite and shape, and creates the physics body
*	Parameters - position x and y, width and height, and the box2d world
*	Return - none
*/
Ropelink::Ropelink(float posX, float posY, float width, float height, b2World* world)
{
	m_position = b2Vec2(posX, posY);
	m_width = width;
	m_height = height;
	
	LoadSprite("Assets/Sprites/log.png");

	// Load the shape
	LoadShape();
	CreatePhysicsBody(world);

	m_type = ROPELINK;
}

/*
*	BirdObj Destructor
*	Parameters - none
*	Return - none
*/
Ropelink::~Ropelink()
{

}

/*
*	Creates a b2Body with a density, friction, and a shape
*	Parameters - the physics world
*	Return - void
*/
void Ropelink::CreatePhysicsBody(b2World* world)
{
	// Create the body definition of the object
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.x / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), m_position.y);
	bodyDef.type = b2_dynamicBody;

	// Create the body in the world
	m_body = world->CreateBody(&bodyDef);

	// Give the object a shape
	b2PolygonShape shape;
	shape.SetAsBox(m_width / 2.0f, m_height / 2.0f);

	// Give the object a fixture with a density of 1
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	fixtureDef.shape = &shape;
	m_body->CreateFixture(&fixtureDef);
}

/*
*	updates the position of the object
*	Parameters - the time of the program
*	Return - void
*/
void Ropelink::Update(float tiem)
{
	m_position = m_body->GetPosition();
}

/*
*	Render - draws the sprite with the correct scale, rotation and position
*	Parameters - none
*	Return - void
*/
void Ropelink::Render()
{
	// bind the program
	m_program->Use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind the texture and set the "tex" uniform in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	m_program->setUniform("tex", 0);
	m_program->setUniform("model",
		glm::scale(glm::mat4(), glm::vec3(((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), 1.0f, 1.0f)) *
		glm::translate(glm::mat4(), glm::vec3(m_position.x * METERSTOUNITS, m_position.y * METERSTOUNITS, 0.0f)) *
		glm::rotate(glm::mat4(), m_body->GetAngle(), glm::vec3(0, 0, 1)));

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
*	Returns the b2Body of the Rope link
*	Parameters - none
*	Return - b2Body pointer
*/
b2Body* Ropelink::GetBody()
{
	return m_body;
}