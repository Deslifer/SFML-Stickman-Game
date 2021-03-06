#include "PixelRope.h"



PixelRope::PixelRope(b2World* world, sf::Sprite sprite, float x, float y, b2BodyType type, float size,b2Body* body2, float size2, float originalScale)
	:Pixel(world,sprite, x, y,type,size)
{
	this->filter1.categoryBits = CATEGORY_BODY;
	this->filter1.maskBits = MASK_BODY;
	this->circle.m_radius = size/2 / SCALE;
	this->bfix = this->body->CreateFixture(&this->circle, 6);
	this->bfix->SetFilterData(filter1);
	this->bfix->SetUserData((void*)"pixelRope");
	this->rDef.bodyA = this->body;
	this->rDef.bodyB = body2;
	this->rDef.collideConnected = true;
	if(size>size2)
		this->rDef.maxLength = size/2/SCALE;
	else
		this->rDef.maxLength = size2 / 2 / SCALE;
	this->rDef.localAnchorA.Set(0, -1 /SCALE);
	this->rDef.localAnchorB.Set(0,  1/SCALE);
	this->world->CreateJoint(&this->rDef);
}

PixelRope::~PixelRope()
{
}

void PixelRope::update(const float& dt)
{
}

