#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include<cstdlib>
#include<sstream>
#include<fstream>
#include <vector>
#include <stack>
#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "SFML/Window.hpp"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "PhysicComponent.h"

enum movement_states {IDLE = 0,MOVING, MOVING_LEFT, MOVING_RIGHT, MOVING_UP,MOVING_DOWN};

class MovementComponent
{
private:
	sf::Sprite& sprite;

	float maxVelocity;
	float acceleration;
	float deceleration;

	sf::Vector2f velocity;

	//Initialazer functions

public:
	MovementComponent(sf::Sprite& sprite, 
		float maxVelocity,float acceleration, float deceleration);
	virtual ~MovementComponent();

	//Accessors
	const sf::Vector2f & getVelocity() const;

	//Functions
	const bool getState(const short unsigned state) const;

	void move(const float dir_x, const float dir_y, const float& dt);
	void update(const float& dt);
	void updateBody(const float& dt, b2Body* body, bool onGround);
};

