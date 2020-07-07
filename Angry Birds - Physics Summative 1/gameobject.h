

#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Local includes
#include "utils.h"
#include "program.h"

enum GameObjectType
{
	BIRDOBJ,
	ENEMY,
	CONSTRUCT,
	ROPE,
	ROPELINK,
	SPRING,
	OTHER
};

class GameObject
{
public:

	GameObject();
	GameObject(float posX, float posY, float width, float height, char* filePath);
	~GameObject();

	virtual void LoadShape();
	void LoadSprite(char* path);

	virtual void Render();
	virtual void Update(float time);

	// Get & set methods
	float GetHalfWidth();
	float GetHalfHeight();
	GameObjectType GetType();

protected:

	b2Vec2 m_position;
	Program* m_program;
	GLuint m_vao, m_vbo, m_texture;

	float m_width, m_height;
	GameObjectType m_type;
};

#endif