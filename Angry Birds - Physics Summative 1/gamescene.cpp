

// This include
#include "gamescene.h"

// Local includes
#include "hud.h"

// Static Variables
GameScene* GameScene::m_gameScene = 0;

/*
*	GameScene Constructor - sets all pointers to null
*	Parameters - none
*	Return - none
*/
GameScene::GameScene() :
	m_world(0),
	m_background(0),
	m_ground(0),
	m_mouseJoint(0),
	m_slingshotBack(0),
	m_slingshotFore(0),
	m_isGameOver(0),
	m_rope(0),
	m_spring(0)
{
	
}

/*
*	GameScene Destructor - frees memory by deleting pointers and calling the reset function
*	Parameters - none
*	Return - none
*/
GameScene::~GameScene()
{
	// Free memory of game objects by calling the reset function
	Reset();

	// Free memory by deleting other objects in the scene
	delete m_background;
	m_background = 0;
	delete m_slingshotBack;
	m_slingshotBack = 0;
	delete m_slingshotFore;
	m_slingshotFore = 0;
}

/*
*	Creates the physics world and the ground body, creats the slingshot objects
*	Parameters - none
*	Return - void
*/
void GameScene::InitialiseWorld()
{
	// Set up the physics world
	m_world = new b2World(b2Vec2(0.0f, -9.81f));
	
	// Create a background
	m_background = new Background("Assets/Sprites/background.png");

	// Create a ground object
	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, -4.5f);

	// Create the body in the world
	m_ground = m_world->CreateBody(&bodyDef);

	// Give the object a shape
	b2PolygonShape shape;
	shape.SetAsBox(UNITSTOMETERS / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), 1.0f);

	// Give the object a fixture with a density of 1
	b2FixtureDef fixtureDef;
	fixtureDef.friction = 1.0f;
	fixtureDef.shape = &shape;
	m_ground->CreateFixture(&fixtureDef);
	
	// Create the slingshot object
	m_slingshotBack = new GameObject(-3.6f, -2.6f, 0.3f, 1.8f, "Assets/Sprites/slingshotback.png");
	m_slingshotFore = new GameObject(-3.6f, -2.6f, 0.3f, 1.8f, "Assets/Sprites/slingshotfore.png");
}

/*
*	Creates the objects for the first level of the game
*	Parameters - none
*	Return - void
*/
void GameScene::SetupLevel1()
{
	m_currentLevel = 1;

	// Add the birds
	AddBirdObj(-3.6f, -2.0f, CLASSIC);
	AddBirdObj(-3.6f, -2.0f, CLASSIC);
	AddBirdObj(-3.6f, -2.0f, CLASSIC);

	// Add the objects
	m_constructs.push_back(new Construct(2.6f, -3.0f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(3.8f, -3.0f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(3.8f, -2.3f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(4.2f, -3.0f, m_world, DESTRUCTIBLE_PLANK, 110.0f));

	// Add the enemies
	m_enemies.push_back(new Enemy(3.2f, -3.0f, 0.4f, 0.55f, m_world, "Assets/Sprites/pig.png"));

	// Update the 'birds left' text in the HUD
	HUD::GetInstance().UpdateBirdsLeftText(m_birds.size());
}

/*
*	Creates the objects for the second level of the game
*	Parameters - none
*	Return - void
*/
void GameScene::SetupLevel2()
{
	m_currentLevel = 2;

	// Add the birds
	AddBirdObj(-3.6f, -2.0f, SPLITTER);
	AddBirdObj(-3.6f, -2.0f, SPLITTER);
	AddBirdObj(-3.6f, -2.0f, SPLITTER);

	// Add the objects
	m_constructs.push_back(new Construct(2.6f, -3.0f, m_world, INDESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(1.15f, -3.0f, m_world, INDESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, -2.3f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(1.15f, -2.3f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(3.8f, -2.3f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));

	// Add the enemies
	m_enemies.push_back(new Enemy(3.2f, -3.0f, 0.4f, 0.55f, m_world, "Assets/Sprites/pig.png"));
	m_enemies.push_back(new Enemy(1.9f, -3.0f, 0.4f, 0.55f, m_world, "Assets/Sprites/pig.png"));


	// Misc objects (Rope)
	m_rope = new Rope(1.42f, -2.0f, m_world, 9, m_constructs[3], m_constructs[2]);

	// Update the 'birds left' text in the HUD
	HUD::GetInstance().UpdateBirdsLeftText(m_birds.size());
}

/*
*	Creates the objects for the third level of the game
*	Parameters - none
*	Return - void
*/
void GameScene::SetupLevel3()
{
	m_currentLevel = 3;

	// Add the birds
	AddBirdObj(-3.6f, -2.0f, BOMBER);
	AddBirdObj(-3.6f, -2.0f, BOMBER);
	AddBirdObj(-3.6f, -2.0f, BOMBER);

	// Add the objects
	m_constructs.push_back(new Construct(2.6f, -3.0f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, -2.5f, m_world, INDESTRUCTIBLE_PLANK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, -2.0f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, -1.5f, m_world, INDESTRUCTIBLE_PLANK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, -1.0f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, -0.5f, m_world, INDESTRUCTIBLE_PLANK, 0.0f));
	m_constructs.push_back(new Construct(2.6f, 0.0f, m_world, DESTRUCTIBLE_BLOCK, 0.0f));

	// Add the enemies
	m_enemies.push_back(new Enemy(3.5f, -3.0f, 0.4f, 0.55f, m_world, "Assets/Sprites/pig.png"));


	// Misc objects (Spring)
	m_spring = new Spring(1.5f, -2.9f, m_world, m_ground);


	// Update the 'birds left' text in the HUD
	HUD::GetInstance().UpdateBirdsLeftText(m_birds.size());
}

/*
*	Calls the reset function to delete level elements, then goes to the next level or loads the same level
*	Parameters - whether or not to reload the same level, reference to the main game state
*	Return - void
*/
void GameScene::GoToNextLevel(bool isSameLevel, GameState& state)
{
	// Reset the game scene by deleting game objects
	Reset();

	// Go to a new level based on the current level
	switch (m_currentLevel)
	{
	case 1:
	{
		if (isSameLevel)
			// If this variable is true, reset the same level instead
			SetupLevel1();
		else
			SetupLevel2();
	}
	break;
	case 2:
	{
		if (isSameLevel)
			SetupLevel2();
		else
			SetupLevel3();
	}
	break;
	case 3:
	{
		if (isSameLevel)
			SetupLevel3();
		else
			state = MENU;
	}
	break;
	default:
	{
		// By default return to the menu
		state = MENU;
	}
	break;
	}
}

/*
*	Updates all game objects in the scene
*	Parameters - the window, the current game time, and the main game state
*	Return - void
*/
void GameScene::Update(GLFWwindow* window, float time, GameState& state)
{


	// Update all game objects
	for (std::vector<Construct*>::iterator it = m_constructs.begin(); it != m_constructs.end();)
	{
		(*it)->Update(time);
		if ((*it)->IsAlive())
			// If the object is alive, advance the iterator
			++it;
		else
		{
			// delete the bird if it's not alive
			m_world->DestroyBody((*it)->GetBody());
			delete (*it);
			(*it) = 0;
			it = m_constructs.erase(it);
		}
	}

	for (std::vector<BirdObj*>::iterator it = m_birds.begin(); it != m_birds.end();)
	{
		(*it)->Update(time);
		if ((*it)->IsAlive())
			// If the object is alive, advance the iterator
			++it;
		else
		{
			// delete the bird if it's not alive
			m_world->DestroyBody((*it)->GetBody());
			delete (*it);
			(*it) = 0;
			it = m_birds.erase(it);
		}
	}

	for (std::vector<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end();)
	{
		(*it)->Update(time);
		if ((*it)->IsAlive())
			// If the object is alive, advance the iterator
			++it;
		else
		{
			// delete the enemy if it's not alive
			m_world->DestroyBody((*it)->GetBody());
			delete (*it);
			(*it) = 0;
			it = m_enemies.erase(it);
		}
	}

	for (std::vector<BirdObj*>::iterator it = m_splitterBirds.begin(); it != m_splitterBirds.end();)
	{
		(*it)->Update(time);
		if ((*it)->IsAlive())
			// If the object is alive, advance the iterator
			++it;
		else
		{
			// delete the enemy if it's not alive
			m_world->DestroyBody((*it)->GetBody());
			delete (*it);
			(*it) = 0;
			it = m_splitterBirds.erase(it);
		}
	}

	// Press the space bar to activate the bird
	if (glfwGetKey(window, GLFW_KEY_SPACE) && !m_birds.empty())
		m_birds.front()->Ability();

	// Update the joint def
	if (m_mouseJoint != 0)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		float posX = ((float)x / (WINDOW_WIDTH / (UNITSTOMETERS * 2))) - UNITSTOMETERS;
		float posY = ((UNITSTOMETERS * 2) - ((float)y / (WINDOW_HEIGHT / (UNITSTOMETERS * 2)))) - UNITSTOMETERS;

		b2Vec2 vec = b2Vec2(posX, posY) - m_slingshotStart;
		if (vec.Length() > 1.0f)
		{
			vec.Normalize();
			b2Vec2 v = (vec + m_slingshotStart);
			m_mouseJoint->SetTarget(b2Vec2(v.x / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), v.y));
		}
		else
			m_mouseJoint->SetTarget(b2Vec2(posX / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), posY));
	}

	// Check win and lose conditions
	if (m_enemies.empty())
		m_isGameOver = 1;
	if (!m_enemies.empty() && m_birds.empty())
		m_isGameOver = 2;

	// Update the physics world
	m_world->Step(1.0f / 60.0f, 8, 3);

	// Destroy the joints that need to be destroyed
	if (!m_destroyJoints.empty())
		m_world->DestroyJoint(m_destroyJoints.front());
	m_destroyJoints.clear();
}

/*
*	Renders all objects in the scene
*	Parameters - the window
*	Return - void
*/
void GameScene::Render(GLFWwindow* window)
{
	// clear everything
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render the background
	m_background->Render();

	// Render all game objects
	m_slingshotBack->Render();


	if (!m_birds.empty())
		m_birds.front()->Render();
	for (std::vector<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end(); ++it)
		(*it)->Render();
	for (std::vector<Construct*>::iterator it = m_constructs.begin(); it != m_constructs.end(); ++it)
		(*it)->Render();
	for (std::vector<BirdObj*>::iterator it = m_splitterBirds.begin(); it != m_splitterBirds.end(); ++it)
		(*it)->Render();

	m_slingshotFore->Render();

	// Render the HUD
	HUD::GetInstance().Render();

	// Swap the display buffers (displays what was just drawn)
	glfwSwapBuffers(window);
}

/*
*	Deletes all physics bodies and objects in the scene
*	Parameters - none
*	Return - void
*/
void GameScene::Reset()
{
	// Clear all the vectors of objects
	for (std::vector<BirdObj*>::iterator it = m_birds.begin(); it != m_birds.end(); ++it)
	{
		// Deleting physics objects and their container objects
		m_world->DestroyBody((*it)->GetBody());
		delete (*it);
		(*it) = 0;
	}
	m_birds.clear();

	for (std::vector<Enemy*>::iterator it = m_enemies.begin(); it != m_enemies.end(); ++it)
	{
		m_world->DestroyBody((*it)->GetBody());
		delete (*it);
		(*it) = 0;
	}
	m_enemies.clear();

	for (std::vector<Construct*>::iterator it = m_constructs.begin(); it != m_constructs.end(); ++it)
	{
		m_world->DestroyBody((*it)->GetBody());
		delete (*it);
		(*it) = 0;
	}
	m_constructs.clear();

	for (std::vector<BirdObj*>::iterator it = m_splitterBirds.begin(); it != m_splitterBirds.end(); ++it)
	{
		m_world->DestroyBody((*it)->GetBody());
		delete (*it);
		(*it) = 0;
	}
	m_splitterBirds.clear();

	// Reset the score for the HUD
	HUD::GetInstance().ResetScore();
	HUD::GetInstance().UpdateBirdsLeftText(m_birds.size());

	// Reset the isGameOver variable
	m_isGameOver = 0;
}

/*
*	Adds a bird to the scene, with a different sprite and type
*	Parameters - position x and y, and the type of bird
*	Return - void
*/
void GameScene::AddBirdObj(float posX, float posY, BirdType type)
{
	// Add a new object to the scene at a given location
	char* filePath;
	switch (type)
	{
		// Load a different sprite for each type of bird
	case BOMBER:
		filePath = "Assets/Sprites/birdbomber.png";
		break;
	case SPLITTER:
		filePath = "Assets/Sprites/birdsplitter.png";
		break;
	default: 
		filePath = "Assets/Sprites/bird.png";
		break;
	}
	m_birds.push_back(new BirdObj(posX, posY, 1.0f, 1.0f, m_world, filePath, type));
}

/*
*	Creates new birds with the same variables as the bird passed in, to create a splitting effect
*	Parameters - the bird to create copies of
*	Return - void
*/
void GameScene::AddSplitterBirds(BirdObj* splitter)
{
	// Add two new birds that split off from a given bird
	m_splitterBirds.push_back(new BirdObj(splitter->GetBody()->GetPosition().x * ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH) + 0.05f, splitter->GetBody()->GetPosition().y, 0.7f, 0.7f, m_world, "Assets/Sprites/birdsplitter.png", SPLITTER));
	
	// Activate the new birds, and set the physics to the same as the original
	m_splitterBirds.back()->Activate();
	m_splitterBirds.back()->CopyVariables(*splitter);
	
	// Split effect achieved by altering the velocity a little bit
	m_splitterBirds.back()->GetBody()->SetLinearVelocity(splitter->GetBody()->GetLinearVelocity() + b2Vec2(1.0f, 0.0f));
	m_splitterBirds.back()->GetBody()->SetAngularVelocity(-splitter->GetBody()->GetAngularVelocity());

	// Repeat with a second bird
	m_splitterBirds.push_back(new BirdObj(splitter->GetBody()->GetPosition().x * ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH) - 0.05f, splitter->GetBody()->GetPosition().y, 0.7f, 0.7f, m_world, "Assets/Sprites/birdsplitter.png", SPLITTER));
	m_splitterBirds.back()->Activate();
	m_splitterBirds.back()->CopyVariables(*splitter);
	m_splitterBirds.back()->GetBody()->SetLinearVelocity(splitter->GetBody()->GetLinearVelocity() + b2Vec2(-1.0f, 0.0f));
	m_splitterBirds.back()->GetBody()->SetAngularVelocity(-splitter->GetBody()->GetAngularVelocity());
}

/*
*	Creates a mouse joint at the target position if the target position is on the mouse
*	Parameters - mouse coordinates 
*	Return - void
*/
void GameScene::CreateMouseJoint(float posX, float posY)
{
	// If the mouse joint is not null and there are birds left
	if (m_mouseJoint == 0 && !m_birds.empty())
	{
		// If the front bird is clicked on and is not yet launched
		if (m_birds.front()->IsMouseHit(posX, posY) && !m_birds.front()->IsLaunched())
		{
			// Activate the physics body of the object
			m_birds.front()->Activate();

			// Create a mosue joint def and adjust settings
			b2MouseJointDef mouseJointDef;
			mouseJointDef.bodyA = m_ground;
			mouseJointDef.bodyB = m_birds.front()->GetBody();
			mouseJointDef.target = b2Vec2(posX / ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH), posY);
			mouseJointDef.collideConnected = true;
			mouseJointDef.maxForce = 10000;

			// Create the mouse joint
			m_mouseJoint = (b2MouseJoint*)m_world->CreateJoint(&mouseJointDef);

			// Save the starting point
			m_slingshotStart = b2Vec2(posX, posY);
		}
	}
}

/*
*	Destroys the current mouse joint and flings the bird in the direction of the slingshot
*	Parameters - mouse coordinates
*	Return - void
*/
void GameScene::ReleaseMouseJoint(float posX, float posY)
{
	if (m_mouseJoint != 0)
	{
		b2Body* b = m_mouseJoint->GetBodyB();

		// Destroy the mouse joint
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = 0;

		// Launch the ball with the slingshot
		b2Vec2 vec = (m_slingshotStart - b2Vec2(posX, posY));
		if (vec.Length() > 5.0f)
			// restrict the velocity of the launch
			vec.Normalize();

		vec *= 6.4f;
		b->ApplyLinearImpulse(vec, b->GetPosition(), true);
		BirdObj* current = static_cast<BirdObj*>(b->GetUserData());
		current->Launch();
		HUD::GetInstance().UpdateBirdsLeftText(m_birds.size() - 1);
	}
}


/*
*	returns the chain object
*	Parameters - none
*	Return - Chain pointer
*/
Rope* GameScene::GetRope()
{
	return m_rope;
}

/*
*	Return whether the game is over, won, or lost
*	Parameters - none
*	Return - unsigned isGameOver variable
*/
unsigned GameScene::GetIsGameOver()
{
	return m_isGameOver;
}



/*
*	Adds a joint to the vector of joints to be destroyed
*	Parameters - b2Joint to be destroyed
*	Return - void
*/
void GameScene::SetDestroyJoint(b2Joint* destroyJoint)
{
	m_destroyJoints.push_back(destroyJoint);
}

/*
*	Returns the singleton instance of the GameScene
*	Parameters - none
*	Return - reference to the gamescene instance
*/
GameScene& GameScene::GetInstance()
{
	// Return the singleton
	if (m_gameScene == 0)
		m_gameScene = new GameScene();

	return *m_gameScene;
}

/*
*	Destroys the singleton instance of the gamescene
*	Parameters - none
*	Return - void
*/
void GameScene::DestroyInstance()
{
	// Delete the singleton instance
	delete m_gameScene;
	m_gameScene = 0;
}