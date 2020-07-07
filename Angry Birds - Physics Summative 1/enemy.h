

#pragma once

#ifndef ENEMY_H
#define ENEMY_H

// Local includes
#include "gameobject.h"

class Enemy : public GameObject
{
public:

	Enemy(float posX, float posY, float width, float height, b2World* world, char* filePath);
	~Enemy();

	virtual void Update(float time);
	virtual void Render();
	void CreatePhysicsBody(b2World* world);

	void Kill();

	b2Body* GetBody();
	b2Vec2 Position();
	bool IsAlive();

private:

	int m_health;
	bool m_isAlive;

	b2Body* m_body;

};

#endif