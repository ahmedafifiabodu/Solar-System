#include "Classes/Explosion.h"
#include "Classes/MyContactListener.h"

std::vector<sf::Texture> Planet::planetTextures;

int main()
{	// Create the SFML window
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Box2D + SFML", sf::Style::Fullscreen);

	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	sf::Texture sunTexture;
	if (!sunTexture.loadFromFile("Assets/sun.jfif")) {
		// Handle error loading texture
		return EXIT_FAILURE;
	}

	b2BodyDef sunBodyDef;
	sunBodyDef.type = b2_staticBody;
	sunBodyDef.position.Set(window.getSize().x / (2.0f * SCALE), window.getSize().y / (2.0f * SCALE));
	b2Body* sunBody = world.CreateBody(&sunBodyDef);

	b2CircleShape sunShape;
	sunShape.m_radius = 50.0f / SCALE;
	b2FixtureDef sunFixtureDef;
	sunFixtureDef.shape = &sunShape;
	sunBody->CreateFixture(&sunFixtureDef);
	float sunRotationAngle = 0.0f;
	float sunRotationSpeed = 0.005f;

	std::vector<Planet> planets;
	std::vector<Explosion> explosions;

	MyContactListener contactListener;
	world.SetContactListener(&contactListener);

	// Load background image
	sf::Texture backgroundTexture;
	sf::Sprite background;

	if (!backgroundTexture.loadFromFile("Assets/1.jpg")) {
		//Handle error loading texture
		return EXIT_FAILURE;
	}

	background.setTexture(backgroundTexture);
	background.setScale(static_cast<float>(window.getSize().x * 1.2f) / backgroundTexture.getSize().x,
		static_cast<float>(window.getSize().y * 1.8f) / backgroundTexture.getSize().y);
	background.setOrigin(background.getLocalBounds().width / 2.0f, background.getLocalBounds().height / 2.0f);
	background.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	for (int i = 1; i <= 9; ++i) {
		sf::Texture planetTexture;
		std::string texturePath = "Assets/Planets/texture" + std::to_string(i) + ".png";
		if (!planetTexture.loadFromFile(texturePath)) {
			// Handle error loading texture
			return EXIT_FAILURE;
		}
		Planet::planetTextures.push_back(planetTexture);
	}

	// Main loop
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			// Create a new planet when the mouse is clicked
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				srand(static_cast<unsigned int>(time(nullptr)));

				int index = rand() % 8;
				int numbers[] = { 30, 40, 50, 60, 70, 80, 90, 100 };
				int randomNumber = numbers[index];
				float randomRadius = static_cast<float>(randomNumber) / SCALE;

				//std::cout << randomRadius<<std::endl;

				planets.emplace_back(world, b2Vec2(mousePosition.x / SCALE, mousePosition.y / SCALE), randomRadius, randomNumber, sunBody);
			}
		}

		// Clear the window
		window.clear(sf::Color::White);

		// Draw the background
		background.rotate(sunRotationSpeed * 1.0f / 60.0f);
		window.draw(background);

		// Step the Box2D world forward in time
		world.Step(1.0f / 60.0f, 8, 3);

		for (auto& planet : planets) {

			if (planet.hasRing) {
				window.draw(planet.ring);
			}

			planet.update(1.0f / 60.0f, sunBody, planets);

			planet.shape.setRotation(planet.rotationAngle * 180.0f / b2_pi);
			window.draw(planet.shape);
		}

		// Collision
		for (auto it = contactListener.explosions.begin(); it != contactListener.explosions.end();) {
			it->update(1.0f / 60.0f);
			window.draw(it->shape);

			if (it->timer <= 0.0f) {
				it = contactListener.explosions.erase(it);
			}
			else {
				++it;
			}
		}

		// Draw the sun at the center of the screen

		// Sun Rotation
		sunRotationAngle += sunRotationSpeed * 1.0f / 60.0f;
		sunShape.m_p = b2Vec2(0.0f, 0.0f);
		sunBody->SetTransform(sunBody->GetPosition(), sunRotationAngle);

		// Sun
		sf::CircleShape sun(50.0f);
		sun.setTexture(&sunTexture);
		sun.setOrigin(50.0f, 50.0f);
		sun.setPosition(sunBody->GetPosition().x * SCALE, sunBody->GetPosition().y * SCALE);
		sun.setRotation(sunRotationAngle * 180.0f / b2_pi);
		window.draw(sun);

		// Display the window
		window.display();
	}

	return 0;
}