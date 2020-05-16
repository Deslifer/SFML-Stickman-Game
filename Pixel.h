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
	float size;
	//SFML
	sf::Color color;
	sf::Sprite sprite;

	//Variables

public:
	Pixel(b2World* world, sf::Sprite sprite, float x, float y, b2BodyType type, float size);
	virtual ~Pixel();
	b2Body* getBody();
	b2CircleShape getShape();
	b2Fixture* getFixture();
	sf::Sprite getSprite();
	float getSize();
	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget* target);
};

