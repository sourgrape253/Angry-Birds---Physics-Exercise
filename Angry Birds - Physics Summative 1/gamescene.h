
#pragma once

#ifndef GAMESCENE_H
#define GAMESCENE_H

// Local includes
#include "utils.h"
#include "bird-obj.h"
#include "background.h"
#include "enemy.h"
#include "construct.h"
#include "textlabel.h"
#include "rope.h"
#include "ropelink.h"
#include "spring.h"


class GameScene
{
public:

	~GameScene();

	static GameScene& GetInstance();
	static void DestroyInstance();

	void InitialiseWorld();
	void Update(GLFWwindow* window, float time, GameState& state);
	void Render(GLFWwindow* window);
	void Reset();

	void SetupLevel1();
	void SetupLevel2();
	void SetupLevel3();
	void GoToNextLevel(bool isSameLevel, GameState& state);

	void AddSplitterBirds(BirdObj* splitter);
	void AddBirdObj(float posX, float posY, BirdType type);
	void CreateMouseJoint(float posX, float posY);
	void ReleaseMouseJoint(float posX, float posY);

	unsigned GetIsGameOver();

	void SetDestroyJoint(b2Joint*);

	Rope* GetRope();

private:

	// Private methods
	GameScene();
	GameScene(const GameScene& other);
	GameScene& operator= (const GameScene& other);

	// Game objects
	std::vector<BirdObj*> m_birds;
	std::vector<BirdObj*> m_splitterBirds;
	std::vector<Enemy*> m_enemies;
	std::vector<Construct*> m_constructs;
	GameObject* m_slingshotFore;
	GameObject* m_slingshotBack;
	Background* m_background;
	unsigned m_isGameOver;

	Rope* m_rope;
	Spring* m_spring;

	std::vector<b2Joint*> m_destroyJoints;

	int m_currentLevel;

	// Physics variables
	b2World* m_world;
	b2Body* m_ground;
	b2MouseJoint* m_mouseJoint;
	b2Vec2 m_slingshotStart;
	

	// Singleton Instance
	static GameScene* m_gameScene;

};

#endif