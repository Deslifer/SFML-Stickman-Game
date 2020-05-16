#include "EntityBox2D.h"

void EntityBox2D::initVariables()
{
	this->movementComponent = NULL;
	this->animationComponent = NULL;
	this->physicComponent = NULL;
	this->audioComponent = NULL;
}

EntityBox2D::EntityBox2D()
{
	this->initVariables();
}

EntityBox2D::~EntityBox2D()
{
	delete this->movementComponent;
	delete this->animationComponent;
	delete this->physicComponent;
	delete this->audioComponent;
}

//Component functions
void EntityBox2D::setTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);
}

void EntityBox2D::createMovementComponent(const float maxVelocity, const float acceleration, const float deceleration)
{
	this->movementComponent = new MovementComponent(this->sprite, maxVelocity, acceleration, deceleration);
}

void EntityBox2D::createAnimationComponent(sf::Texture& texture_sheet)
{
	this->animationComponent = new AnimationComponent(this->sprite, texture_sheet);
}

void EntityBox2D::createPhysicComponent()
{
	this->physicComponent = new PhysicComponent(this->world,this->sprite);
}

void EntityBox2D::createAudioComponent()
{
	this->audioComponent = new AudioComponent();
}

//Functions
void EntityBox2D::setPosition(const float x, const float y)
{

	this->sprite.setPosition(x, y);

}

void EntityBox2D::move(const float dir_x, const float dir_y, const float& dt)
{

	if (this->movementComponent)
	{
		this->movementComponent->move(dir_x, dir_y, dt); //Sets velocity
	}
}

void EntityBox2D::update(const float& dt)
{

}

void EntityBox2D::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
