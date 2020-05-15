#pragma once

#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "PhysicComponent.h"

class EntityBox2D
{
private:
	void initVariables();
protected:

	sf::Sprite sprite;

	float spriteSizeX;
	float spriteSizeY;

	MovementComponent* movementComponent;
	AnimationComponent* animationComponent;
	PhysicComponent* physicComponent;
	b2World* world = nullptr;

public:
	EntityBox2D();
	virtual ~EntityBox2D();
	//Component functions
	void setTexture(sf::Texture& texture);
	void createMovementComponent(const float maxVelocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& texture_sheet);
	void createPhysicComponent();
	//Functions

	virtual void setPosition(const float x, const float y);
	virtual void move(const float x, const float y, const float& dt);


	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget* target);
};

