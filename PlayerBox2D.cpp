#include "PlayerBox2D.h"


//Initialazer functions
void PlayerBox2D::initVariables()
{

}

void PlayerBox2D::initComponents()
{
}

//Constructors / Destructors
PlayerBox2D::PlayerBox2D(b2World* world,float x, float y, sf::Texture& texture_sheet)
{
	this->world = world;
	this->initVariables();

	this->setPosition(x, y);

	this->initComponents();

	this->createMovementComponent(100.f, 15.f, 5.f);
	this->createAnimationComponent(texture_sheet);
	this->sprite.setOrigin(43.f, 81.5f);
	this->animationComponent->addAnimation("IDLE_RIGHT", 0.5f, 0, 0, 9, 0, 86, 163);
	this->animationComponent->addAnimation("WALK_RIGHT", 0.5f, 0, 1, 9, 6, 86, 163);
	this->createPhysicComponent();

}

PlayerBox2D::~PlayerBox2D()
{
}

//Functions
void PlayerBox2D::update(const float& dt)
{

	this->movementComponent->updateBody(dt,this->physicComponent->getBody(),this->physicComponent->onGround());

	if (this->movementComponent->getState(IDLE))
		this->animationComponent->play("IDLE_RIGHT", dt);
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		this->sprite.setScale(1.f, 1.f);
		this->animationComponent->play("WALK_RIGHT", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		this->sprite.setScale(-1.f, 1.f);
		this->animationComponent->play("WALK_RIGHT", dt);
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("IDLE_RIGHT", dt);
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("IDLE_RIGHT", dt);
	}


	this->sprite.setPosition(this->physicComponent->getPosition().x, this->physicComponent->getPosition().y);
	//this->sprite.setRotation(this->physicComponent->getAngle());

}

sf::Sprite* PlayerBox2D::getSprite()
{
	return &this->sprite;
}

sf::Vector2f PlayerBox2D::getPosition()
{
	return this->sprite.getPosition();
}

b2Body* PlayerBox2D::getBody()
{
	return this->physicComponent->getBody();
}

