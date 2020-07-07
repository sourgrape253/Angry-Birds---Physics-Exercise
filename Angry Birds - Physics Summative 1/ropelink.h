
#pragma once

#ifndef RPOELINK_H
#define ROPELINK_H

// Local includes
#include "gameobject.h"

class Ropelink : public GameObject
{
public:

	Ropelink(float posX, float posY, float width, float height, b2World* world);
	~Ropelink();

	void CreatePhysicsBody(b2World* world);
	virtual void Update(float time);
	virtual void Render();

	b2Body* GetBody();

private:

	b2Body* m_body;

};

#endif