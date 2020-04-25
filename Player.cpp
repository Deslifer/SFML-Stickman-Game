#include "Player.h"


//Initialazer functions
void Player::initVariables()
{

}

void Player::initComponents()
{
}

//Constructors / Destructors
Player::Player(float x,float y,sf::Texture& texture_sheet)
{
	this->initVariables();

	this->setPosition(x, y);

	this->initComponents();

	this->createMovementComponent(300.f, 15.f, 5.f);
	this->createAnimationComponent(texture_sheet);
	this->sprite.setOrigin(43.f, 81.5f);
	this->animationComponent->addAnimation("IDLE_RIGHT", 0.5f, 0, 0, 9, 0, 86, 163);
	this->animationComponent->addAnimation("WALK_RIGHT", 0.5f, 0, 1, 9, 6, 86, 163);
}

Player::~Player()
{
}

//Functions
void Player::update(const float& dt)
{		
	this->movementComponent->update(dt);

	if(this->movementComponent->getState(IDLE))
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
		this->animationComponent->play("WALK_RIGHT", dt);
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK_RIGHT", dt);
	}

}

sf::Sprite Player::getSprite()
{
	return this->sprite;
}

