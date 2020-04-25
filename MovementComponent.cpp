#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Sprite& sprite,
	float maxVelocity,float acceleration,float deceleration)
	: sprite(sprite),maxVelocity(maxVelocity),
	acceleration(acceleration),deceleration(deceleration)
{
	
}

MovementComponent::~MovementComponent()
{

}

const sf::Vector2f & MovementComponent::getVelocity() const
{
	return this->velocity;
}

//Functions
const bool MovementComponent::getState(const short unsigned state) const
{
	switch (state)
	{
	case IDLE:
		if (this->velocity.x == 0.f && this->velocity.y == 0.f)
			return true;
		break;
	case MOVING:
		if (this->velocity.x != 0.f || this->velocity.y != 0.f)
			return true;
		break;
	case MOVING_LEFT:
		if (this->velocity.x < 0.f)
			return true;
		break;
	case MOVING_RIGHT:
		if (this->velocity.x > 0.f)
			return true;
		break;
	case MOVING_UP:
		if (this->velocity.y < 0.f)
			return true;
		break;
	case MOVING_DOWN:
		if (this->velocity.y > 0.f)
			return true;
		break;
	}
	return false;
}

void MovementComponent::move(const float dir_x, const float dir_y,const float& dt)
{
	/*Accelerating a sprite until reaches the max velocity*/ 
	this->velocity.x += this->acceleration * dir_x;
	this->velocity.y += this->acceleration * dir_y;


	
}

void MovementComponent::update(const float& dt)
{
	/*Decelerates the sprite and controls the maximum velocity.
	Moves the sprite
	*/
	if (this->velocity.x > 0.f) //Check for positive x
	{
		//Max velocity check
		if (this->velocity.x > this->maxVelocity)
			this->velocity.x = this->maxVelocity;

		//Deceleration
		this->velocity.x -= deceleration;
		if (this->velocity.x < 0.f)
			this->velocity.x = 0.f;
	}
	else if (this->velocity.x < 0.f) //Check for negative x
	{
		//Max velocity check
		if (this->velocity.x < -this->maxVelocity)
			this->velocity.x = -this->maxVelocity;

		//Deceleration
		this->velocity.x += deceleration;
		if (this->velocity.x > 0.f)
			this->velocity.x = 0.f;
	}

	if (this->velocity.y > 0.f) //Check for positive y
	{
		//Max velocity check
		if (this->velocity.y > this->maxVelocity)
			this->velocity.y = this->maxVelocity;

		//Deceleration
		this->velocity.y -= deceleration;
		if (this->velocity.y < 0.f)
			this->velocity.y = 0.f;
	}
	else if (this->velocity.y < 0.f) //Check for negative y
	{
		//Max velocity check
		if (this->velocity.y < -this->maxVelocity)
			this->velocity.y = -this->maxVelocity;

		//Deceleration
		this->velocity.y += deceleration;
		if (this->velocity.y > 0.f)
			this->velocity.y = 0.f;
	}

	//Final move
	this->sprite.move(this->velocity * dt); //Uses velocity
}

void MovementComponent::updateBody(const float& dt, b2Body* body, bool onGround)
{
	/*Decelerates the sprite and controls the maximum velocity.
	Moves the sprite
	*/
	if (this->velocity.x > 0.f) //Check for positive x
	{
		//Max velocity check
		if (this->velocity.x > this->maxVelocity)
			this->velocity.x = this->maxVelocity;

		//Deceleration
		this->velocity.x -= deceleration;
		if (this->velocity.x < 0.f)
			this->velocity.x = 0.f;
	}
	else if (this->velocity.x < 0.f) //Check for negative x
	{
		//Max velocity check
		if (this->velocity.x < -this->maxVelocity)
			this->velocity.x = -this->maxVelocity;

		//Deceleration
		this->velocity.x += deceleration;
		if (this->velocity.x > 0.f)
			this->velocity.x = 0.f;
	}

	if (this->velocity.y > 0.f) //Check for positive y
	{
		//Max velocity check
		if (this->velocity.y > this->maxVelocity)
			this->velocity.y = this->maxVelocity;

		//Deceleration
		this->velocity.y -= deceleration;
		if (this->velocity.y < 0.f)
			this->velocity.y = 0.f;
	}
	else if (this->velocity.y < 0.f) //Check for negative y
	{
		//Max velocity check
		if (this->velocity.y < -this->maxVelocity)
			this->velocity.y = -this->maxVelocity;

		//Deceleration
		this->velocity.y += deceleration;
		if (this->velocity.y > 0.f)
			this->velocity.y = 0.f;
	}


	/*b2Vec2 vel = body->GetLinearVelocity();
	//vel.x = this->velocity.x/SCALE;
	//body->SetLinearVelocity(vel);
	float velChange = this->velocity.x/SCALE -vel.x;
	float force = velChange / (1 / 60.0); //f = mv/t
	body->ApplyForce(b2Vec2(force, 0), body->GetWorldCenter(),true);

	if (this->getState(MOVING_UP) && onGround)
	{
		body->SetLinearVelocity(b2Vec2(vel.x, -10));
	}*/

	b2Vec2 vel = body->GetLinearVelocity();
	float  angVel = body->GetAngularVelocity();
	float velChange = this->velocity.x / SCALE - vel.x;
	float force = body->GetMass()*velChange / (1 / 60.0);
	if (this->getState(MOVING_RIGHT)) 
		if (vel.x < maxVelocity/SCALE) 
		{ 
			body->ApplyForce(b2Vec2(force, 0), body->GetWorldCenter(), true);
		}
	if (this->getState(MOVING_LEFT) ) 
		if (vel.x > -maxVelocity / SCALE) 
		{
			body->ApplyForce(b2Vec2(force, 0), body->GetWorldCenter(), true);
		}
	if (this->getState(MOVING_UP))  
		if (onGround) 
			body->SetLinearVelocity(b2Vec2(vel.x, -10));

	

	//Final move
	//this->sprite.move(this->velocity * dt); //Uses velocity
}
