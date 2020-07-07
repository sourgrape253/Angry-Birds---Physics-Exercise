#pragma once

#ifndef CONSTRUCT_H
#define CONSTRUCT_H

// Local includes
#include "gameobject.h"

enum ConstructType
{
	INDESTRUCTIBLE_PLANK,
	DESTRUCTIBLE_PLANK,
	INDESTRUCTIBLE_BLOCK,
	DESTRUCTIBLE_BLOCK
};

class Construct : public GameObject
{
public:

	Construct(float posX, float posY, b2World* world, ConstructType type, float angle);
	~Construct();

	void CreatePhysicsBody(b2World* world, float angle);
	virtual void Render();
	virtual void Update(float time);

	void TakeDamage(int damage, ConstructType type);
	void ChangeSprite(ConstructType type);
	bool IsAlive();
	ConstructType GetConstructType();
	b2Body* GetBody();

private:

	ConstructType m_constructType;
	b2Body* m_body;
	int m_health;
	int m_currentHealth;
	bool m_isAlive;

};

#endif