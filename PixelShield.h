#pragma once
#include "Pixel.h"
class PixelShield :
	public Pixel
{
private:
	b2MotorJointDef mDef;
public:
	PixelShield(b2World* world, sf::Sprite sprite, float x, float y, b2BodyType type, float size, b2Body* player, float originalScale);
	virtual ~PixelShield();

	void update(const float& dt);
};

