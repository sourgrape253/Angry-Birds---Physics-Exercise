

// This include
#include "contactlistener.h"

// Local includes
#include "enemy.h"
#include "construct.h"
#include "rope.h"
#include "gamescene.h"

/*
*	ContactListener Constructor
*	Parameters - none
*	Return - none
*/
ContactListener::ContactListener()
{

}

/*
*	ContactListener Destructor
*	Parameters - none
*	Return - none
*/
ContactListener::~ContactListener()
{

}

/*
*	Occurs when collision happens, but before it resolves. Uses velocities before colision to calculate damage ot objects
*	Parameters - b2Contact, b2Manifold
*	Return - void
*/
void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	// Get collision variables
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2PointState state1[2], state2[2];
	b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
	if (state2[0] == b2_addState)
	{
		// Find the resulting velocity of the collision
		b2Body* bodyA = contact->GetFixtureA()->GetBody();
		b2Body* bodyB = contact->GetFixtureB()->GetBody();
		b2Vec2 point = worldManifold.points[0];
		b2Vec2 vA = bodyA->GetLinearVelocityFromWorldPoint(point);
		b2Vec2 vB = bodyB->GetLinearVelocityFromWorldPoint(point);
		float32 approachVelocity = b2Dot(vB - vA, worldManifold.normal);

		// if the collision was with and enemy and has enough force, kill the enemy
		if (GetBodyType(bodyA) == ENEMY && GetBodyType(bodyB) == BIRDOBJ && abs(approachVelocity) > 2.0f)
		{
			Enemy* e = static_cast<Enemy*>(bodyA->GetUserData());
			if (e != NULL)
				e->Kill();
		}
		if (GetBodyType(bodyB) == ENEMY && GetBodyType(bodyA) == BIRDOBJ && abs(approachVelocity) > 2.0f)
		{
			Enemy* e = static_cast<Enemy*>(bodyB->GetUserData());
			if (e != NULL)
				e->Kill();
		}

		// If it collides with a building object, damage the object
		if (GetBodyType(bodyA) == CONSTRUCT && GetBodyType(bodyB) == BIRDOBJ)
		{
			Construct* e = static_cast<Construct*>(bodyA->GetUserData());
			if (e != NULL)
				e->TakeDamage((int)approachVelocity, e->GetConstructType());
		}
		if (GetBodyType(bodyB) == BIRDOBJ && GetBodyType(bodyA) == CONSTRUCT)
		{
			Construct* e = static_cast<Construct*>(bodyB->GetUserData());
			if (e != NULL)
				e->TakeDamage((int)approachVelocity, e->GetConstructType());
		}

		// If a bird collides with a Rope with enough force, break the Rope
		if (GetBodyType(bodyA) == ROPE && GetBodyType(bodyB) == BIRDOBJ && abs(approachVelocity) > 2.0f)
		{
			GameScene::GetInstance().GetRope()->BreakRope();
		}
		if (GetBodyType(bodyB) == ROPE && GetBodyType(bodyA) == BIRDOBJ && abs(approachVelocity) > 2.0f)
		{
			GameScene::GetInstance().GetRope()->BreakRope();
		}
	}
}

/*
*	Converts the body user data from the collision into a GameObject to determine the type of object
*	Parameters - collision body
*	Return - GameObjectType enumerator
*/
GameObjectType ContactListener::GetBodyType(b2Body* body)
{
	// cast the user data to a game object to find the type
	GameObject* object = static_cast<GameObject*>(body->GetUserData());
	if (object != NULL)
		return object->GetType();
	return OTHER;
}