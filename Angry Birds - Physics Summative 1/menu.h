

#pragma once

#ifndef MENU_H
#define MENU_H

// Local includes
#include "utils.h"
#include "textlabel.h"
#include "background.h"
#include "gameobject.h"

class Menu
{
public:

	~Menu();

	static Menu& GetInstance();
	static void DestroyInstance();

	void Initialise();
	void Render(GLFWwindow* window);
	void CheckIfClicked(GameState& state, float mouseX, float mouseY, GLFWwindow* window);

private:

	// Private methods
	Menu();
	Menu(const Menu& other);
	Menu& operator= (const Menu& other);

	// Text labels
	TextLabel* m_startText;
	TextLabel* m_exitText;
	Background* m_background;

	// Singleton Instance
	static Menu* m_menu;

};

#endif