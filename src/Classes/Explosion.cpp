#include "Explosion.h"

Explosion::Explosion(const b2Vec2& position)
{
	shape.setRadius(30.0f);
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(30.0f, 30.0f);
	shape.setPosition(position.x * SCALE, position.y * SCALE);
	timer = 0.5f;
}

void Explosion::update(float deltaTime)
{
	timer -= deltaTime;
}