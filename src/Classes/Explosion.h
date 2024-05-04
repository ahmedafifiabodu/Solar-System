#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "SCALE.h"

class Explosion
{
public:
	sf::CircleShape shape;
	float timer;

	Explosion(const b2Vec2& position);
	void update(float deltaTime);
};

#endif // EXPLOSION_H
