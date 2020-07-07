

//This include
#include "Spring.h"

/*
*	Spring Constructor - Sets the variables, loads the shape, sprite and physics body. Also creates the spring gameobject
*	Parameters - the position x and y, the physics world, the ground body
*	Return - none
*/
Spring::Spring(float posX, float posY, b2World* world, b2Body* ground)
{
	m_position = b2Vec2(posX, posY);
	m_width = 1.0f;
	m_height = 0.5f;

	LoadSprite("Assets/Sprites/springtop.png");

	// Load the shape
	LoadShape();
	CreatePhysicsBody(world, ground);

	m_type = SPRING;

	// Initialise the spring sprite object
	m_spring = new GameObject(posX, posY - 0.3f, 0.4f, 0.6f, "Assets/Sprites/Springspring.png");
}

/*
*	Spring Destructor
*	Parameters - none
*	Return - none
*/
Spring::~Spring()
{

}

/*
*	Creates the body of the platform, and creates a prismatic joint to the ground
*	Parameters - the physics world, and the ground body
*	Return - void
*/
void Spring::CreatePhysicsBody(b2World* world, b2Body* ground)
{
	// Create the body definition of the object
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.x / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), m_position.y);
	bodyDef.type = b2_dynamicBody;

	// Create the body in the world
	m_body = world->CreateBody(&bodyDef);

	// Give the object a shape
	b2PolygonShape shape;
	shape.SetAsBox(m_width / 2.0f, m_height / 4.0f);

	// Give the object a fixture with natural restitution
	b2FixtureDef fixtureDef;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.f;
	fixtureDef.restitution = 1.5f;

	fixtureDef.shape = &shape;
	m_body->CreateFixture(&fixtureDef);

	// Create a prismatic joint with the ground
	b2PrismaticJointDef jointDef;
	jointDef.Initialize(m_body, ground, m_body->GetPosition(), b2Vec2(0.f, 1.f));
	jointDef.collideConnected = true;
	jointDef.lowerTranslation = -1.5f;
	jointDef.upperTranslation = 1.5f;
	jointDef.enableLimit = true;
	jointDef.enableMotor = true;

	m_joint = static_cast<b2PrismaticJoint*>(world->CreateJoint(&jointDef));
}

/*
*	Updates the position, and controls the oscillation of the platform
*	Parameters - the current game time
*	Return - void
*/
void Spring::Update(float time)
{
	// Control the oscillation of the spring
	m_joint->SetMaxMotorForce(abs((m_joint->GetJointTranslation() * 1000) + (m_joint->GetJointSpeed() * 5)));
	m_joint->SetMotorSpeed(m_joint->GetJointTranslation() > 0.0f ? -10000.0f : 10000.0f);

	m_position = m_body->GetPosition();
}

/*
*	Render the spring game object, and the spring platform
*	Parameters - none
*	Return - void
*/
void Spring::Render()
{
	m_spring->Render();

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
*	Returns the physics body
*	Parameters - none
*	Return - b2Body pointer
*/
b2Body* Spring::GetBody()
{
	return m_body;
}