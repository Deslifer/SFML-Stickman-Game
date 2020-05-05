#include "PixelSword.h"

PixelSword::PixelSword(b2World* world, float x, float y, b2BodyType type, float size, b2Body* player)
	:Pixel(world, x, y, type, size)
{
	this->filter1.categoryBits = CATEGORY_BODY;
	this->filter1.maskBits = MASK_BODY_SHIELD;
	this->circle.m_radius = size / 2 / SCALE;
	this->bfix = this->body->CreateFixture(&this->circle, 0.1);
	this->bfix->SetFilterData(filter1);
	this->bfix->SetUserData((void*)"pixelSword");
	this->mDef2.bodyA = this->body;
	this->mDef2.bodyB = player;
	this->mDef2.collideConnected = true;
	this->mDef2.maxForce = -1;
	this->world->CreateJoint(&this->mDef2);
}

PixelSword::~PixelSword()
{
}

void PixelSword::update(const float& dt)
{
}
