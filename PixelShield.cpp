#include "PixelShield.h"

PixelShield::PixelShield(b2World* world, float x, float y, b2BodyType type, float size,b2Body* player)
	:Pixel(world, x, y, type, size)
{
	this->filter1.categoryBits = CATEGORY_BODY;
	this->filter1.maskBits = MASK_BODY_SHIELD;
	this->circle.m_radius = size / 2 / SCALE;
	this->bfix = this->body->CreateFixture(&this->circle, 0.1);
	this->bfix->SetFilterData(filter1);
	this->bfix->SetUserData((void*)"pixelShield");
	this->mDef.bodyA = this->body;
	this->mDef.bodyB = player;
	this->mDef.collideConnected = true;
	this->mDef.maxForce = 1;
	this->world->CreateJoint(&this->mDef);
}

PixelShield::~PixelShield()
{
}

void PixelShield::update(const float& dt)
{
}
