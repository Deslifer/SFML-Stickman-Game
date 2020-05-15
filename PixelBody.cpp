#include "PixelBody.h"

PixelBody::PixelBody(b2World* world, sf::Sprite sprite, float x, float y, b2BodyType type, float size, b2Body* body2,float originalScale)
	:Pixel(world, sprite, x, y, type, size)
{
	this->filter1.categoryBits = CATEGORY_BODY;
	this->filter1.maskBits = MASK_BODY;
	this->circle.m_radius = size / 2 / SCALE;
	this->bfix = this->body->CreateFixture(&this->circle, 6);
	this->bfix->SetFilterData(filter1);
	this->bfix->SetUserData((void*)"pixelBody");

}

PixelBody::~PixelBody()
{

}

void PixelBody::update(const float& dt)
{
}
