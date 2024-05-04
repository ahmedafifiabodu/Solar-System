#include "Planet.h"
#include <iostream>

Planet::Planet(b2World& world, const b2Vec2& position, float radius, int randomRadiusIndex, b2Body* sunBody)
	: rotationAngle(0.0f), angularVelocity(0.01f)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = position;
	body = world.CreateBody(&bodyDef);

	b2CircleShape planetShape;
	planetShape.m_radius = radius;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &planetShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.2f;
	body->CreateFixture(&fixtureDef);

	shape.setRadius(radius * SCALE);

	setTextureBasedOnRadius(randomRadiusIndex);

	shape.setOrigin(radius * SCALE, radius * SCALE);

	// Initialize the ring
	ring.setFillColor(sf::Color::Transparent); // Set the ring color
	ring.setOutlineThickness(2.0f);
	ring.setOutlineColor(sf::Color::White);
	ring.setOrigin(0.0f, 0.0f); // Set the origin to the top-left corner

	// Calculate the distance between the planet and the sun
	float distanceToSun = (body->GetPosition() - sunBody->GetPosition()).Length() * SCALE;

	// Update the radius of the ring based on the distance
	ring.setRadius(distanceToSun);
	ring.setOrigin(distanceToSun, distanceToSun);
	ring.setPosition(sunBody->GetPosition().x * SCALE, sunBody->GetPosition().y * SCALE);

	hasRing = true;
}

void Planet::update(float deltaTime, b2Body* sunBody, const std::vector<Planet>& planets)
{
	b2Vec2 sunToPlanet = body->GetPosition() - sunBody->GetPosition();
	float distance = sunToPlanet.Length() * SCALE;

	//std::cout << "distance" << distance << std::endl;	

	// I increase the speed for the sake of the demonstration
	float rotationSpeed = 0.8f / (distance * body->GetFixtureList()->GetShape()->m_radius);
	std::cout << "rotationSpeed " << body->GetFixtureList()->GetShape()->m_radius << std::endl;

	float angle = std::atan2(sunToPlanet.y, sunToPlanet.x);
	angle += rotationSpeed * deltaTime;

	body->SetTransform(sunBody->GetPosition() + b2Vec2(std::cos(angle) * distance / SCALE, std::sin(angle) * distance / SCALE), 0);
	shape.setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);

	rotationAngle += angularVelocity * deltaTime;

	// Update planet positions based on collisions
	for (const auto& otherPlanet : planets)
	{
		if (&otherPlanet != this)
		{
			b2Vec2 planetToPlanet = body->GetPosition() - otherPlanet.body->GetPosition();
			float combinedRadius = body->GetFixtureList()->GetShape()->m_radius +
				otherPlanet.body->GetFixtureList()->GetShape()->m_radius;
			float separation = planetToPlanet.Length() - combinedRadius;

			if (separation < 0)
			{
				// Planets are colliding, remove the ring
				hasRing = false;

				// Optionally, you can set the ring's radius to zero
				ring.setRadius(0.0f);

				// Or make the ring transparent
				ring.setFillColor(sf::Color::Transparent);

				// Planets are colliding, apply a repulsive force
				float impulseMagnitude = 0.08f;
				planetToPlanet.Normalize();
				body->ApplyLinearImpulse(impulseMagnitude * planetToPlanet, body->GetWorldCenter(), true);
			}
		}
	}
}

void Planet::setTextureBasedOnRadius(float radius) {
	int textureIndex;
	if (radius == 30.0f) {
		textureIndex = 8;
	}
	else if (radius == 40.0f) {
		textureIndex = 0;
	}
	else if (radius == 50.0f) {
		textureIndex = 3;
	}
	else if (radius == 60.0f) {
		textureIndex = 1;
	}
	else if (radius == 70.0f) {
		textureIndex = 2;
	}
	else if (radius == 80.0f) {
		textureIndex = 7;
	}
	else if (radius == 90.0f) {
		textureIndex = 5;
	}
	else if (radius == 100.0f) {
		textureIndex = 4;
	}
	else {
		// If the radius doesn't match any condition, use a default texture (e.g., texture 0)
		textureIndex = 0;
	}

	shape.setTexture(&planetTextures[textureIndex]);
}