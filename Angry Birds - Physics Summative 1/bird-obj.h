
#pragma once

#ifndef BIRDOBJ_H
#define BIRDOBJ_H

// Local includes
#include "gameobject.h"

// Constants
#define ALIVETIME 6.0f

enum BirdType
{
	CLASSIC,
	BOMBER,
	SPLITTER
};

class BirdObj : public GameObject
{
public:

	BirdObj(float posX, float posY, float width, float height, b2World* world, char* filePath, BirdType type);
	~BirdObj();

	virtual void Update(float time);
	virtual void Render();
	void CreatePhysicsBody(b2World* world);
	bool IsMouseHit(float posX, float posY);

	void Ability();
	void CopyVariables(const BirdObj& other);

	b2Body* GetBody();
	b2Vec2 Position();
	BirdType GetBirdType();

	void Activate();
	void Launch();
	bool IsAlive();
	bool IsLaunched();

private:

	float m_launchTime;
	bool m_isLaunched;
	bool m_isAlive;
	bool m_isAbilityUsed;

	b2Body* m_body;
	BirdType m_birdType;

};

#endif