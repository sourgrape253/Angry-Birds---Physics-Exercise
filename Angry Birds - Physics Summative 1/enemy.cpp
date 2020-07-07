

// This include
#include "enemy.h"

// Local includes
#include "hud.h"

/*
*	Enemy Constructor - Calls GameObject Constructor and assigns variables, and creates the physics body
*	Parameters - position x and y, width and height, physics world, and file path of the sprite
*	Return - none
*/
Enemy::Enemy(float posX, float posY, float width, float height, b2World* world, char* filePath) :
	GameObject(posX, posY, width, height, filePath),
	m_health(5),
	m_isAlive(true),
	m_body(0)
{
	CreatePhysicsBody(world);

	m_type = ENEMY;
}


Enemy::~Enemy()
{

}

/*
*	updates the position
*	Parameters - current game time
*	Return - void
*/
void Enemy::Update(float time)
{
	m_position = m_body->GetPosition();
}

/*
*	Render - draws the sprite with the correct scale, rotation and position
*	Parameters - none
*	Return - void
*/
void Enemy::Render()
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
*	Creates a b2Body with a density, friction, and a shape
*	Parameters - the physics world
*	Return - void
*/
void Enemy::CreatePhysicsBody(b2World* world)
{
	// Create the body definition of the object
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.x / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), m_position.y);
	bodyDef.type = b2_dynamicBody;

	// Create the body in the world
	m_body = world->CreateBody(&bodyDef);

	// Give the object a shape
	b2CircleShape shape;
	shape.m_radius = m_width / 2.0f;

	// Give the object a fixture with a density of 1
	b2FixtureDef fixtureDef;
	fixtureDef.density = 10.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.shape = &shape;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearDamping(1.0f);

	m_body->SetUserData(this);
}

/*
*	Adds 50 to the score and sets the alive boolean to false
*	Parameters - none
*	Return - void
*/
void Enemy::Kill()
{
	// add to the score and set the object to dead
	HUD::GetInstance().AddScoreText(50);
	m_isAlive = false;
}

/*
*	returns the b2Body
*	Parameters - none
*	Return - b2Body pointer
*/
b2Body* Enemy::GetBody()
{
	return m_body;
}

/*
*	returns the position of the b2Body
*	Parameters - none
*	Return - b2Vec2
*/
b2Vec2 Enemy::Position()
{
	return m_body->GetPosition();
}

/*
*	returns whether the bird is alive or not
*	Parameters - none
*	Return - isAlive boolean
*/
bool Enemy::IsAlive()
{
	return m_isAlive;
}