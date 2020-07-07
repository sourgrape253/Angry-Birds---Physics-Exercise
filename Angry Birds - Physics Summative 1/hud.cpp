

// This include
#include "hud.h"

// Local includes
#include "gamescene.h"

// Static Variables
HUD* HUD::m_HUD = 0;

/*
*	HUD Constructor - sets the score to 0
*	Parameters - none
*	Return - none
*/
HUD::HUD() : 
	m_score(0)
{
	
}

/*
*	HUD Destructor - deletes pointers to free memory
*	Parameters - none
*	Return - none
*/
HUD::~HUD()
{
	// Free memory
	delete m_scoreText;
	m_scoreText = 0;
	delete m_birdsLeftText;
	m_birdsLeftText = 0;
	delete m_birdImage;
	m_birdImage = 0;
	delete m_exit;
	m_exit = 0;
	delete m_restart;
	m_restart = 0;
}

/*
*	Creates all the textlabels and the gameobjects necessary to the HUD
*	Parameters - none
*	Return - void
*/
void HUD::Initialise()
{
	// General text labels
	m_scoreText = new TextLabel("0", glm::vec2(85.0f, 85.0f), 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), "Assets/Fonts/BADABB__.TTF");
	m_birdsLeftText = new TextLabel("4", glm::vec2(-2.0f, 85.0f), 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), "Assets/Fonts/BADABB__.TTF");
	m_birdImage = new GameObject(-0.4f, 4.45f, 0.4f, 0.7f, "Assets/Sprites/bird.png");
	m_exit = new GameObject(-4.7f, 4.43f, 0.3f, 0.5f, "Assets/Sprites/exit.png");
	m_restart = new GameObject(-4.2f, 4.47f, 0.33f, 0.58f, "Assets/Sprites/restart.png");

	// Win/lose text stuff
	m_clickToGoNextText = new TextLabel("Click for next level...", glm::vec2(-18.0f, 5.0f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), "Assets/Fonts/BADABB__.TTF");
	m_clickToResetText = new TextLabel("Click to restart level...", glm::vec2(-20.0f, 5.0f), 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), "Assets/Fonts/BADABB__.TTF");
	m_winText = new TextLabel("YOU WIN!", glm::vec2(-23.0f, 20.0f), 3.0f, glm::vec3(0.0f, 0.0f, 0.0f), "Assets/Fonts/BADABB__.TTF");
	m_loseText = new TextLabel("YOU LOSE!", glm::vec2(-26.0f, 20.0f), 3.0f, glm::vec3(0.0f, 0.0f, 0.0f), "Assets/Fonts/BADABB__.TTF");
}

/*
*	Renders all text labels and gameobjects in the HUD
*	Parameters - none
*	Return - void
*/
void HUD::Render()
{
	// Render all HUD items
	m_scoreText->Render();
	m_birdsLeftText->Render();
	m_birdImage->Render();
	m_exit->Render();
	m_restart->Render();

	// If the game is over, display correct message
	if (GameScene::GetInstance().GetIsGameOver() == 1)
	{
		m_clickToGoNextText->Render();
		m_winText->Render();
	}
	else if (GameScene::GetInstance().GetIsGameOver() == 2)
	{
		m_clickToResetText->Render();
		m_loseText->Render();
	}
}

/*
*	checks if the mouse was clicked on a button in the HUD, and calls functions appropriately
*	Parameters - reference to the game state, mouse coordinates
*	Return - void
*/
void HUD::CheckIfClicked(GameState& state, float mouseX, float mouseY)
{
	// If the game is over, any click advances the game
	if (GameScene::GetInstance().GetIsGameOver() == 2)
	{
		// If they lost, reset the same level
		GameScene::GetInstance().GoToNextLevel(true, state);
	}
	else if (GameScene::GetInstance().GetIsGameOver() == 1)
	{
		// If they won, go to the next level
		GameScene::GetInstance().GoToNextLevel(false, state);
	}
	else
	{
		if (mouseX > 20 && mouseX < 57 && mouseY > 22 && mouseY < 60)
		{
			// If quit button is clicked, go to the menu
			state = MENU;
			GameScene::GetInstance().Reset();
		}
		if (mouseX > 86 && mouseX < 118 && mouseY > 20 && mouseY < 58)
		{
			// If reset button is clicked, reset the same level
			GameScene::GetInstance().GoToNextLevel(true, state);
		}
	}
}

/*
*	sets the text of the textlabel to the current number of birds left
*	Parameters - number of birds left
*	Return - void
*/
void HUD::UpdateBirdsLeftText(unsigned birdsLeft)
{
	m_birdsLeftText->SetText(std::to_string(birdsLeft));
}

/*
*	adds to the game score, and sets the text label to match the current score
*	Parameters - number to add to the score
*	Return - void
*/
void HUD::AddScoreText(int score)
{
	// Update the score and the text
	m_score += score;
	m_scoreText->SetText(std::to_string(m_score));
}

/*
*	sets the score to 0, and sets the text label to match the current score
*	Parameters - none
*	Return - void
*/
void HUD::ResetScore()
{
	// Reset the score to 0 and update the text
	m_score = 0;
	m_scoreText->SetText(std::to_string(m_score));
}

/*
*	Returns the singleton instance of the HUD
*	Parameters - none
*	Return - reference to the HUD instance
*/
HUD& HUD::GetInstance()
{
	// Return the singleton
	if (m_HUD == 0)
		m_HUD = new HUD();

	return *m_HUD;
}

/*
*	Destroys the singleton instance of the HUD
*	Parameters - none
*	Return - void
*/
void HUD::DestroyInstance()
{
	// Delete the singleton instance
	delete m_HUD;
	m_HUD = 0;
}