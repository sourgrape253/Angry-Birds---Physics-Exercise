

#pragma once

// Third-party includes
#include "Dependencies\glew\glew.h"
#include "Dependencies\GLFW\glfw3.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\Box2D\Box2D\Box2D.h"

#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"

// Library includes
#include <iostream>
#include <sstream>

// Constants
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define XUNITS WINDOW_WIDTH / WINDOW_HEIGHT

#define UNITSTOMETERS 5.0f
#define METERSTOUNITS 1.0f / UNITSTOMETERS

#define DEGREESTORADIANS 0.0174533f
#define RADIANSTODEGREES 1.0f / DEGREESTORADIANS

// Enums
enum GameState
{
	MENU,
	GAME
};