// MyContactListener.cpp
#include "MyContactListener.h"

void MyContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	// Check if bodies are planets
	if (bodyA->GetType() == b2_dynamicBody && bodyB->GetType() == b2_dynamicBody) {
		b2Vec2 contactPoint = contact->GetManifold()->points[0].localPoint;
		explosions.emplace_back(bodyA->GetPosition() + contactPoint);
	}
}