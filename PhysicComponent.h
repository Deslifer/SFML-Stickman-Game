#pragma once

#include <iostream>
#include <string>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "SFML/Window.hpp"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "box2d/box2d.h"

// 0000000000000001 in binary
static short CATEGORY_BODY = 0x0001;
static short CATEGORY_PLATFORM = 0x0002;

// 0000000000000010 in binary
static short CATEGORY_NOT_BODY = 0x0004;
static short CATEGORY_BODY_SHIELD = 0x0008;
static short CATEGORY_PLAYER = 0x0010;
static short CATEGORY_PIXEL = 0x0012;

static short MASK_BODY = CATEGORY_PLAYER|CATEGORY_BODY | CATEGORY_PLATFORM;
static short MASK_BODY_SHIELD = CATEGORY_PLAYER|CATEGORY_BODY | CATEGORY_PLATFORM | CATEGORY_BODY_SHIELD;
static short MASK_BODY_WITHOUT_PLATFORM = CATEGORY_BODY;
static short MASK_PLAYER= CATEGORY_BODY | CATEGORY_PLATFORM;
static short MASK_PLATFORM = CATEGORY_PLAYER|CATEGORY_BODY;
static short MASK_PIXEL = CATEGORY_PLAYER | CATEGORY_BODY| CATEGORY_PLATFORM;
static short MASK_NOT_BODY = 0;

class PhysicComponent
{
private:
	sf::Sprite& sprite;
	float width;
	float height;
	b2World* world = nullptr;
	b2PolygonShape shape;
	b2BodyDef bdef;
	b2Body* body = nullptr;
	const float SCALE = 30.f;
	const float DEG = 57.29577f;

public:
	PhysicComponent(b2World* world, sf::Sprite& sprite);
	virtual ~PhysicComponent();
	sf::Vector2f getPosition();
	b2Vec2 getVelocity();
	float  getAngle(); 
	b2Body* getBody();
	bool onGround();
};

