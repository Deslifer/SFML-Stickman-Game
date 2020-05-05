#pragma once
#include "Pixel.h"
class PixelShield :
	public Pixel
{
private:
	b2MotorJointDef mDef;
public:
	PixelShield(b2World* world, float x, float y, b2BodyType type, float size, b2Body* player);
	virtual ~PixelShield();

	void update(const float& dt);
};

