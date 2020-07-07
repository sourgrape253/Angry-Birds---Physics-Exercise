
#pragma once

#ifndef HUD_H
#define HUD_H

// Local includes
#include "utils.h"
#include "textlabel.h"
#include "gameobject.h"

class HUD
{
public:

	~HUD();

	static HUD& GetInstance();
	static void DestroyInstance();

	void Initialise();
	void UpdateBirdsLeftText(unsigned birdsLeft);
	void AddScoreText(int score);
	void ResetScore();
	void CheckIfClicked(GameState& state, float mouseX, float mouseY);

	void Render();

private:

	// Private methods
	HUD();
	HUD(const HUD& other);
	HUD& operator= (const HUD& other);

	// Text labels
	TextLabel* m_birdsLeftText;
	TextLabel* m_scoreText;
	GameObject* m_exit;
	GameObject* m_restart;
	GameObject* m_birdImage;
	int m_score;

	// Winning and losing stuff
	TextLabel* m_loseText;
	TextLabel* m_winText;
	TextLabel* m_clickToResetText;
	TextLabel* m_clickToGoNextText;

	// Singleton Instance
	static HUD* m_HUD;

};

#endif