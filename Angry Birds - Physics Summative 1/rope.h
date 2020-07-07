
#pragma once

#ifndef ROPE_H
#define ROPE_H

// Local includes
#include "ropelink.h"
#include "construct.h"

class Rope : public GameObject
{
public:

	Rope(float posX, float posY, b2World* world, int length, Construct* fix, Construct* fix2 = NULL);
	~Rope();

	void CreateRope(int length, Construct* fix, Construct* fix2);
	virtual void Render();
	virtual void Update(float time);

	void BreakRope();
	std::vector<Ropelink*> GetLinks();

private:

	b2World& m_worldRef;
	std::vector<Ropelink*> m_links;
	bool m_ropeBroken;

};

#endif