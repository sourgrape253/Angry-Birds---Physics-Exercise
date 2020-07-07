#pragma once

#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

// Local includes
#include "utils.h"
#include "gameobject.h"

class ContactListener : public b2ContactListener
{
public:

	ContactListener();
	~ContactListener();

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

private:

	GameObjectType GetBodyType(b2Body* body);

};

#endif