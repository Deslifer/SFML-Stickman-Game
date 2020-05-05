#pragma once
#include "Box2D/box2d.h"
#include <SFML/Graphics.hpp>
#include "PhysicComponent.h"

class Pixel
{
protected:
	//box2d
	b2World* world;
	b2Body* body;
	b2BodyDef bdef;
	b2CircleShape circle;
	b2Filter filter1;
	b2Filter filter2;
	b2Fixture* bfix;
	//SFML
	sf::Color color;

	//Variables
	const float SCALE = 30.f;
	const float DEG = 57.29577f;

public:
	Pixel(b2World* world, float x, float y, b2BodyType type, float size);
	virtual ~Pixel();
	b2Body* getBody();
	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget* target,sf::Sprite sprite);
};

