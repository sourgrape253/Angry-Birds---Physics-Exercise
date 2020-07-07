

//This include
#include "construct.h"

// Local includes
#include "hud.h"

/*
*	Construct Constructor - sets variables depending on type, creates the physics body, and loads shape and sprite
*	Parameters - position x and y, physics world, type of object, the starting rotation angle
*	Return - none
*/
Construct::Construct(float posX, float posY, b2World* world, ConstructType type, float angle) :
	m_constructType(type),
	m_isAlive(true)
{
	m_position = b2Vec2(posX, posY);

	// Load a different shape and image depending on the type of object
	switch (type)
	{
	case INDESTRUCTIBLE_PLANK:
	{
		m_width = 1.5f;
		m_height = 0.2f;
		m_health = 100;

		LoadSprite("Assets/Sprites/indestructable_plank.png");
	}
	break;
	case INDESTRUCTIBLE_BLOCK:
	{
		m_width = 0.8f;
		m_height = 0.8f;
		m_health = 100;

		LoadSprite("Assets/Sprites/indestructable_block.png");
	}
	break;
	case DESTRUCTIBLE_PLANK:
	{
		m_width = 1.5f;
		m_height = 0.2f;
		m_health = 8;

		LoadSprite("Assets/Sprites/destructable_plank.png");
	}
	break;
	case DESTRUCTIBLE_BLOCK:
	{
		m_width = 0.8f;
		m_height = 0.8f;
		m_health = 10;

		LoadSprite("Assets/Sprites/destructable_block.png");
	}
	break;
	default: break;
	}
	
	// Load the shape
	LoadShape();
	CreatePhysicsBody(world, angle);
	m_currentHealth = m_health;

	m_type = CONSTRUCT;
}

/*
*	Construct Destructor
*	Parameters - none
*	Return - none
*/
Construct::~Construct()
{

}

/*
*	Creates a b2Body with a density depending on the type, friction, and a shape, and adjust the starting transform
*	Parameters - the physics world, starting rotation angle
*	Return - void
*/
void Construct::CreatePhysicsBody(b2World* world, float angle)
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

	b2FixtureDef fixtureDef;

	// Give the fixtureDef different values based on the type
	switch (m_constructType)
	{
	case INDESTRUCTIBLE_PLANK:
	{
		fixtureDef.density = 5.0f;
	}
	break;
	case INDESTRUCTIBLE_BLOCK:
	{
		fixtureDef.density = 10.0f;
	}
	break;
	case DESTRUCTIBLE_PLANK:
	{
		fixtureDef.density = 0.5f;
	}
	break;
	case DESTRUCTIBLE_BLOCK:
	{
		fixtureDef.density = 2.0f;
	}
	break;
	default: break;
	}

	fixtureDef.friction = 0.3f;

	// Create the body
	fixtureDef.shape = &shape;
	m_body->CreateFixture(&fixtureDef);

	// Set the starting angle
	m_body->SetTransform(m_body->GetPosition(), angle * DEGREESTORADIANS);

	m_body->SetUserData(this);
}

/*
*	updates the position, and checks if the object should be dead
*	Parameters - the current game time
*	Return - void
*/
void Construct::Update(float time)
{
	m_position = m_body->GetPosition();

	// Check if the object is dead
	if (m_currentHealth <= 0)
	{
		m_isAlive = false;
		HUD::GetInstance().AddScoreText(10);
	}
}

/*
*	Render - draws the sprite with the correct scale, rotation and position
*	Parameters - none
*	Return - void
*/
void Construct::Render()
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
*	loads a new sprite for the object based on the type of object and whether it is destructable or not
*	Parameters - the type of this object
*	Return - void
*/
void Construct::ChangeSprite(ConstructType type)
{
	// Switch the sprites of destructable items
	switch (type)
	{
	case DESTRUCTIBLE_PLANK:
	{
		LoadSprite("Assets/Sprites/destructable_plank_damaged.png");
	}
	break;
	case DESTRUCTIBLE_BLOCK:
	{
		LoadSprite("Assets/Sprites/destructable_block_damaged.png");
	}
	break;
	default: break;
	}
}

/*
*	Takes away from the objects health based on the damage value passed in
*	Parameters - the damage taken, and the type of this object
*	Return - void
*/
void Construct::TakeDamage(int damage, ConstructType type)
{
	// subract the damage taken from the total health
	m_currentHealth -= abs(damage);
	if (m_currentHealth <= m_health / 2)
	{
		// Change the sprite if the object is below half health
		ChangeSprite(type);
	}
}

/*
*	Returns whether the object is alive or not
*	Parameters - none
*	Return - isAlive boolean
*/
bool Construct::IsAlive()
{
	return m_isAlive;
}

/*
*	Returns the construct type of this object
*	Parameters - none
*	Return - ConstructType enumerator
*/
ConstructType Construct::GetConstructType()
{
	return m_constructType;
}

/*
*	Returns the physics body of the object
*	Parameters - none
*	Return - b2Body pointer
*/
b2Body* Construct::GetBody()
{
	return m_body;
}