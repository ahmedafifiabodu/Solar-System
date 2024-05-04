#pragma once

#include "Planet.h"
#include "Explosion.h"

class MyContactListener : public b2ContactListener
{
public:
	std::vector<Explosion> explosions;
	void BeginContact(b2Contact* contact) override;
};
