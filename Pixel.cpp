#include "Pixel.h"
#include <random>
std::default_random_engine generator2;
std::uniform_int_distribution<int> distribution2(0, 255);

Pixel::Pixel(b2World* world, sf::Sprite sprite, float x, float y,b2BodyType type,float size)
{
	this->sprite = sprite;
	this->size = size;
	this->world = world;
	this->bdef.type = type;
	this->bdef.position.Set(x / SCALE, y / SCALE);
	this->body = this->world->CreateBody(&this->bdef);
	this->circle.m_radius = size / SCALE;
	this->filter1.categoryBits = CATEGORY_NOT_BODY;
	this->filter1.maskBits = MASK_NOT_BODY;
	this->bfix = this->body->CreateFixture(&this->circle, 0);
	this->bfix->SetFilterData(this->filter1);
	this->bfix->SetUserData((void*)"pixelAura");
	this->body->SetUserData((void*)"pixel");
	this->color = sf::Color(distribution2(generator2), distribution2(generator2), distribution2(generator2));
	
}

Pixel::~Pixel()
{
}

b2Body* Pixel::getBody()
{
	return this->body;
}

b2CircleShape Pixel::getShape()
{
	return this->circle;
}

b2Fixture* Pixel::getFixture()
{
	return this->bfix;
}

float Pixel::getSize()
{
	return this->size;
}

void Pixel::update(const float& dt)
{
}

void Pixel::render(sf::RenderTarget* target)
{
	b2Vec2 pos = this->body->GetPosition();
	float angle = this->body->GetAngle();
	sprite.setColor(this->color);
	sprite.setPosition(pos.x * SCALE, pos.y * SCALE);
	sprite.setRotation(angle * DEG);
	target->draw(sprite);
}
