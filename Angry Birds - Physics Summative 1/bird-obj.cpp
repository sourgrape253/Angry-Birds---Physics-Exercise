

// This include
#include "bird-obj.h"

// Local includes
#include "gamescene.h"

/*
*	BirdObj Constructor - Sets variables, calls the constructor of GameObject, and creates the physics body
*	Parameters - position x and y, width and height, the box2d world, file path of the sprite and the type of bird
*	Return - none
*/
BirdObj::BirdObj(float posX, float posY, float width, float height, b2World* world, char* filePath, BirdType type) :
	GameObject(posX, posY, width, height, filePath),
	m_isAlive(true),
	m_isLaunched(false),
	m_isAbilityUsed(false),
	m_body(0),
	m_birdType(type)
{
	CreatePhysicsBody(world);

	// Start the body deactivated
	m_body->SetActive(false);

	m_type = BIRDOBJ;
}

/*
*	Background Destructor
*	Parameters - none
*	Return - none
*/
BirdObj::~BirdObj()
{

}

/*
*	Update - updates the position, check whether the bird should be dead or not
*	Parameters - current time of the program
*	Return - void
*/
void BirdObj::Update(float time)
{
	m_position = m_body->GetPosition();

	// if the bird has been launched, and has lived for 6 seconds, kill it
	if (m_isLaunched)
		if ((float)glfwGetTime() - m_launchTime >= ALIVETIME)
		{
			m_isAlive = false;
		}
}

/*
*	Render - draws the sprite with the correct scale, rotation and position
*	Parameters - none
*	Return - void
*/
void BirdObj::Render()
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
*	Performs an action depending on which type of bird it is
*	Parameters - none
*	Return - void
*/
void BirdObj::Ability()
{
	// Activate the ability if the bird has one
	if (m_isLaunched && !m_isAbilityUsed)
	{
		switch (m_birdType)
		{
		case BOMBER:
		{
			m_body->SetGravityScale(2.5f);
		}
		break;
		case SPLITTER:
		{
			GameScene::GetInstance().AddSplitterBirds(this);
		}
		break;
		default: break;
		}
		// Set this variable to true to prevent multiple calls
		m_isAbilityUsed = true;
	}
}

/*
*	Copies certain variables, used for creating birds when splitting
*	Parameters - the bird object you are copying the variables to
*	Return - void
*/
void BirdObj::CopyVariables(const BirdObj& other)
{
	// copies several variables to another bird, used for splitter birds
	this->m_isAlive = other.m_isAlive;
	this->m_launchTime = other.m_launchTime;
	this->m_isLaunched = other.m_isLaunched;
}

/*
*	Creates a b2Body with a density, friction, and a shape
*	Parameters - the physics world
*	Return - void
*/
void BirdObj::CreatePhysicsBody(b2World* world)
{
	// Create the body definition of the object
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.x / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), m_position.y);
	bodyDef.type = b2_dynamicBody;

	// Create the body in the world
	m_body = world->CreateBody(&bodyDef);
	
	// Give the object a shape
	b2CircleShape shape;
	shape.m_radius = (m_width / 2.0f) - 0.1f;

	// Give the object a fixture with a density of 1
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.shape = &shape;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetLinearDamping(0.5f);

	m_body->SetUserData(this);
}

/*
*	returns the b2Body
*	Parameters - none
*	Return - b2Body pointer
*/
b2Body* BirdObj::GetBody()
{
	return m_body;
}

/*
*	returns the type of bird that it is
*	Parameters - none
*	Return - BirdType enumerator
*/
BirdType BirdObj::GetBirdType()
{
	return m_birdType;
}

/*
*	returns the position of the b2Body
*	Parameters - none
*	Return - b2Vec2
*/
b2Vec2 BirdObj::Position()
{
	return m_body->GetPosition();
}

/*
*	activates the b2Body
*	Parameters - none
*	Return - void
*/
void BirdObj::Activate()
{
	m_body->SetActive(true);
}

/*
*	sets the launch boolean to true
*	Parameters - none
*	Return - void
*/
void BirdObj::Launch()
{
	// start the alive timer
	m_isLaunched = true;
	m_launchTime = (float)glfwGetTime();
}

/*
*	returns whether the bird is alive or not
*	Parameters - none
*	Return - isAlive boolean
*/
bool BirdObj::IsAlive()
{
	return m_isAlive;
}

/*
*	returns whether the bird has been launched or not
*	Parameters - none
*	Return - islaunched boolean
*/
bool BirdObj::IsLaunched()
{
	return m_isLaunched;
}

/*
*	detects whether a given mouse position is inside the bird shape
*	Parameters - mouse coordinates
*	Return - boolean - if mouse collision
*/
bool BirdObj::IsMouseHit(float posX, float posY)
{
	// If the mouse is within the bounds of the shape
	return (posX / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH) > m_position.x - m_width / 2.0f &&
		posX / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH) < m_position.x + m_width / 2.0f &&
		posY > m_position.y - m_height / 2.0f &&
		posY < m_position.y + m_height / 2.0f)
		? true : false;
}