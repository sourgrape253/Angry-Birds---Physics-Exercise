

// This include
#include "rope.h"

// Local includes
#include "ropelink.h"
#include "gamescene.h"

/*
*	Cain Constructor - Sets variables and creates the Rope links and joints
*	Parameters - position x and y, the box2d world, length of the Rope, object(s) to attach the Rope to
*	Return - none
*/
Rope::Rope(float posX, float posY, b2World* world, int length, Construct* fix, Construct* fix2) :
	m_ropeBroken(false),
	m_worldRef(*world)
{
	m_position = b2Vec2(posX, posY);
	m_width = 0.2f;
	m_height = 0.2f;

	CreateRope(length, fix, fix2);

	m_type = ROPE;
}

/*
*	Rope Destructor
*	Parameters - none
*	Return - none
*/
Rope::~Rope()
{
	for (std::vector<Ropelink*>::iterator it = m_links.begin(); it != m_links.end(); ++it)
	{
		// Delete all the bodies of the Ropelinks
		m_worldRef.DestroyBody((*it)->GetBody());
		delete (*it);
		(*it) = 0;
	}
	m_links.clear();
}

/*
*	Creates Rope links, and joins them using revolute joints. Attaches the start and end to given objects
*	Parameters - length of the Rope, object(s) to attach the ends to
*	Return - void
*/
void Rope::CreateRope(int length, Construct* fix, Construct* fix2)
{
	// Create start link
	m_links.push_back(new Ropelink(m_position.x, m_position.y, m_width, m_height, &m_worldRef));
	m_links.back()->GetBody()->SetUserData(this);

	// Attach it to the first construct object passed in
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(fix->GetBody(), m_links.back()->GetBody(), m_links.back()->GetBody()->GetPosition());
	jointDef.collideConnected = false;
	jointDef.maxMotorTorque = 100000.0f;
	jointDef.motorSpeed = 2.0f;
	b2RevoluteJoint* joint = static_cast<b2RevoluteJoint*>(m_worldRef.CreateJoint(&jointDef));

	for (int i = 0; i < length; ++i)
	{
		// Create a new Rope link
		Ropelink* newRope = new Ropelink(m_position.x + (i * m_width * ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH)), m_position.y /*+ (i * m_height)*/, m_width, m_height, &m_worldRef);
		newRope->GetBody()->SetUserData(this);
		b2RevoluteJointDef jointDef; 
		jointDef.Initialize(newRope->GetBody(), m_links.back()->GetBody(), newRope->GetBody()->GetPosition());
		jointDef.collideConnected = false;
		jointDef.maxMotorTorque = 100000.0f;
		jointDef.motorSpeed = 2.0f;
		
		// Create a revolute joint to join the Rope links
		b2RevoluteJoint* joint = static_cast<b2RevoluteJoint*>(m_worldRef.CreateJoint(&jointDef));

		m_links.push_back(newRope);
	}

	if (fix2 != NULL)
	{
		// Create end link if another construct was passed in
		b2RevoluteJointDef jointDef2;
		jointDef2.Initialize(m_links.back()->GetBody(), fix2->GetBody(), m_links.back()->GetBody()->GetPosition());
		jointDef2.collideConnected = false;
		jointDef2.maxMotorTorque = 100000.0f;
		jointDef2.motorSpeed = 2.0f;
		b2RevoluteJoint* joint2 = static_cast<b2RevoluteJoint*>(m_worldRef.CreateJoint(&jointDef2));
	}

}

/*
*	BirdObj Constructor - Sets variables, calls the constructor of GameObject, and creates the physics body
*	Parameters - position x and y, width and height, the box2d world, file path of the sprite and the type of bird
*	Return - none
*/
void Rope::Update(float time)
{
	// Update all Rope links
	for (std::vector<Ropelink*>::iterator it = m_links.begin(); it != m_links.end(); ++it)
		(*it)->Update(time);
}

/*
*	Renders the individual Rope links
*	Parameters - none
*	Return - void
*/
void Rope::Render()
{
	// Render all Rope links
	for (std::vector<Ropelink*>::iterator it = m_links.begin(); it != m_links.end(); ++it)
		(*it)->Render();
}

/*
*	destroys the joint in the middle of the Rope, effectively breaking it in half
*	Parameters - none
*	Return - void
*/
void Rope::BreakRope()
{
	// If the Rope is not already broken
	if (!m_ropeBroken)
	{
		// Destroy the joint between the middle links
		b2Joint* destroyJoint = m_links[m_links.size() / 2]->GetBody()->GetJointList()[0].joint;
		GameScene::GetInstance().SetDestroyJoint(destroyJoint);
		m_ropeBroken = true;
	}
}

/*
*	returns the vector of Rope links
*	Parameters - none
*	Return - the vector of Rope links
*/
std::vector<Ropelink*> Rope::GetLinks()
{
	return m_links;
}