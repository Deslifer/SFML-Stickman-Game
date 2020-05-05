#include "PixelBody.h"

PixelBody::PixelBody(b2World* world, float x, float y, b2BodyType type, float size,b2Body* body2)
	:Pixel(world, x, y, type, size)
{
	this->filter1.categoryBits = CATEGORY_BODY;
	this->filter1.maskBits = MASK_BODY;
	this->circle.m_radius = size / 2 / SCALE;
	this->bfix = this->body->CreateFixture(&this->circle, 6);
	this->bfix->SetFilterData(filter1);
	this->bfix->SetUserData((void*)"pixelPoint");

	
	wDef.bodyA = this->body;
	wDef.bodyB = body2;
	wDef.bodyA->SetFixedRotation(true);
	wDef.bodyB->SetFixedRotation(true);
	float dx, dy;
	dx = wDef.bodyA->GetPosition().x - wDef.bodyB->GetPosition().x;
	dy = wDef.bodyA->GetPosition().y - wDef.bodyB->GetPosition().y;
	wDef.collideConnected = true;
	wDef.localAnchorA.Set(0, 0);
	wDef.localAnchorB.Set(dx / 1.2, dy / 1.2);
	this->world->CreateJoint(&wDef);
}

PixelBody::~PixelBody()
{
	
}

void PixelBody::update(const float& dt)
{
}
