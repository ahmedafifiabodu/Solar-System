#ifndef PLANET_H
#define PLANET_H

#include "SCALE.h"
#include <cmath>

class Planet
{
public:

	static std::vector<sf::Texture> planetTextures;
	b2Body* body;
	sf::CircleShape shape;
	sf::CircleShape ring;
	bool hasRing;

	float rotationAngle;
	float angularVelocity;

	Planet(b2World& world, const b2Vec2& position, float radius, int randomRadiusIndex, b2Body* sunBody);

	void update(float deltaTime, b2Body* sunBody, const std::vector<Planet>& planets);
	void setTextureBasedOnRadius(float radius);
};

#endif // PLANET_H