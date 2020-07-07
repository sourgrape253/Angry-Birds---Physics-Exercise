
// Local includes
#include "utils.h"
#include "gamescene.h"
#include "menu.h"
#include "hud.h"

// Global variables
GLFWwindow* g_window = 0;
GameScene& g_gameScene = GameScene::GetInstance();
Menu& g_menu = Menu::GetInstance();
HUD& g_hud = HUD::GetInstance();
GameState g_state = MENU;

/*
*	Catches errors
*	Parameters - the error code and the related message
*	Return - void
*/
void OnError(int errorCode, const char* msg)
{
	throw std::runtime_error(msg);
}

/*
*	Sets up the window, sets window settings, centers the window, and sets the window icon
*	Parameters - none
*	Return - void
*/
void InitialiseGLFW()
{
	// open a window with GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	g_window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Angry Birds", NULL, NULL);
	if (!g_window)
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");

	// GLFW settings
	glfwMakeContextCurrent(g_window);

	// Center the window on the screen
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(g_window, (mode->width - WINDOW_WIDTH) / 2, (mode->height - WINDOW_HEIGHT) / 2);
	
	// Make icon
	int width, height;
	GLFWimage images[1];

	unsigned char* image = SOIL_load_image("Assets/Sprites/angry-bird-icon.png",
		&width,
		&height,
		0,
		SOIL_LOAD_RGBA);
	images[0] = { width, height, image };

	// Set the icon
	glfwSetWindowIcon(g_window, 1, images);
}

/*
*	Sets up glew
*	Parameters - none
*	Return - void
*/
void InitialiseGlew()
{
	// Glew initialisation
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");

	// GLEW throws some errors, so discard all the errors so far
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

/*
*	Called when the mouse is clicked, calls appropriate mouse related functions
*	Parameters - window, mouse button, mouse action, and mods
*	Return - void
*/
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Get time to make sure the reset button isn't triggered too many times
	static double lastTime = glfwGetTime();
	double thisTime = glfwGetTime();

	static float clickCooldown = 0.5f;
	clickCooldown = b2Max(0.0f, clickCooldown - (float)(thisTime - lastTime));

	if (clickCooldown == 0.0f)
	{
		// Get cursor position and translate into meters
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		if (g_state == GAME)
		{
			float posX = (((float)x / (WINDOW_WIDTH / (UNITSTOMETERS * 2))) - UNITSTOMETERS);
			float posY = ((UNITSTOMETERS * 2) - ((float)y / (WINDOW_HEIGHT / (UNITSTOMETERS * 2)))) - UNITSTOMETERS;

			// Call an action based on the mouse action
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
				g_gameScene.CreateMouseJoint(posX, posY);
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
				g_gameScene.ReleaseMouseJoint(posX, posY);

			g_hud.CheckIfClicked(g_state, (float)x, (float)y);
		}
		else if (g_state == MENU)
		{
			g_menu.CheckIfClicked(g_state, (float)x, (float)y, g_window);
		}
		clickCooldown = 0.5f;
	}
	lastTime = thisTime;
}

/*
*	Sets up game, and runs the main game loop
*	Parameters - parameters
*	Return - int
*/
int main(int argc, char *argv[])
{
	// Catches runtime error
	glfwSetErrorCallback(OnError);
	if (!glfwInit())
		throw std::runtime_error("glfwInit failed");

	InitialiseGLFW();
	InitialiseGlew();

	// Initialise game scene, menu, and hud
	g_gameScene.InitialiseWorld();
	g_menu.Initialise();
	g_hud.Initialise();

	// Set callabacks
	glfwSetMouseButtonCallback(g_window, MouseButtonCallback);

	// Variable to hold the time
	double lastTime = glfwGetTime();
	// Main game loop
	while (!glfwWindowShouldClose(g_window))
	{
		// process pending events
		glfwPollEvents();

		// Update and render the game scene
		double thisTime = glfwGetTime();
		if (g_state == GAME)
		{
			g_gameScene.Update(g_window, (float)(thisTime - lastTime), g_state);
			g_gameScene.Render(g_window);
		}
		if (g_state == MENU)
		{
			g_menu.Render(g_window);
		}
		lastTime = thisTime;

		// check for errors
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cerr << "OpenGL Error " << error << std::endl;

		//exit program if escape key is pressed
		if (glfwGetKey(g_window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(g_window, GL_TRUE);
	}

	// clean up and exit
	g_gameScene.DestroyInstance();
	g_menu.DestroyInstance();
	g_hud.DestroyInstance();
	glfwDestroyWindow(g_window);
	glfwTerminate();
	return 0;
}