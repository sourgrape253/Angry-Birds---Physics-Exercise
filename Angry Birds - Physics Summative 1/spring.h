

#pragma once

#ifndef SPRING_H
#define SPRING_H

// Local includes
#include "gameobject.h"

class Spring : public GameObject
{
public:

	Spring(float posX, float posY, b2World* world, b2Body* ground);
	~Spring();

	void CreatePhysicsBody(b2World* world, b2Body* ground);
	virtual void Update(float time);
	virtual void Render();

	b2Body* GetBody();

private:

	b2Body* m_body;
	b2PrismaticJoint* m_joint;
	GameObject* m_spring;

};

#endif