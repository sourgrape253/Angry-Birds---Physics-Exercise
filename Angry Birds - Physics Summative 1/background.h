
#pragma once

#ifndef BACKGROUND_H
#define BACKGROUND_H

// Local includes
#include "utils.h"
#include "gameobject.h"

class Background : public GameObject
{
public:

	Background(char* filePath);
	~Background();

	virtual void LoadShape();

private:

};

#endif