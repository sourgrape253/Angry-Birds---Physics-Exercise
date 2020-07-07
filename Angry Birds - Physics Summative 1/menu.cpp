

// This include
#include "menu.h"

// Local includes
#include "gamescene.h"

// Static Variables
Menu* Menu::m_menu = 0;

/*
*	Menu Constructor
*	Parameters - none
*	Return - none
*/
Menu::Menu()
{
	
}

/*
*	Menu Destructor - deletes the background and textlabels
*	Parameters - none
*	Return - none
*/
Menu::~Menu()
{
	delete m_background;
	m_background = 0;
	delete m_startText;
	m_startText = 0;
	delete m_exitText;
	m_exitText = 0;
}

/*
*	Creates all the menu elements
*	Parameters - none
*	Return - void
*/
void Menu::Initialise()
{
	// Initialise menu objects
	m_background = new Background("Assets/Sprites/menu background.png");
	m_startText = new TextLabel("START", glm::vec2(-15.0f, 15.0f), 3.0f, glm::vec3(1.0f, 1.0f, 1.0f), "Assets/Fonts/BADABB__.TTF");
	m_exitText = new TextLabel("EXIT", glm::vec2(-11.0f, -5.0f), 3.0f, glm::vec3(1.0f, 1.0f, 1.0f), "Assets/Fonts/BADABB__.TTF");
}

/*
*	renders all the menu elements
*	Parameters - the window
*	Return - void
*/
void Menu::Render(GLFWwindow* window)
{
	// clear everything
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	m_background->Render();
	m_startText->Render();
	m_exitText->Render();

	// Swap the display buffers (displays what was just drawn)
	glfwSwapBuffers(window);
}

/*
*	Check if the mouse clicked a menu button, and calls functions appropriately
*	Parameters - reference to game state, mouse coordinates, and the window
*	Return - void
*/
void Menu::CheckIfClicked(GameState& state, float mouseX, float mouseY, GLFWwindow* window)
{
	if (mouseX > 520 && mouseX < 710 && mouseY > 270 && mouseY < 315)
	{
		// If start button was clicked, start the first level
		GameScene::GetInstance().SetupLevel1();
		state = GAME;
	}
	if (mouseX > 565 && mouseX < 695 && mouseY > 345 && mouseY < 375)
		// If exit button was clicked, exit the application
		glfwSetWindowShouldClose(window, GL_TRUE);
}

/*
*	Returns the singleton instance of the menu
*	Parameters - none
*	Return - reference to the menu instance
*/
Menu& Menu::GetInstance()
{
	// Return the singleton
	if (m_menu == 0)
		m_menu = new Menu();

	return *m_menu;
}

/*
*	Destroys the singleton instance of the menu
*	Parameters - none
*	Return - void
*/
void Menu::DestroyInstance()
{
	// Delete the singleton instance
	delete m_menu;
	m_menu = 0;
}