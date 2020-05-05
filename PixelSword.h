#pragma once
#include "Pixel.h"
class PixelSword :
	public Pixel
{
private:
	b2MotorJointDef mDef2;
public:
	PixelSword(b2World* world, float x, float y, b2BodyType type, float size, b2Body* player);
	virtual ~PixelSword();

	void update(const float& dt);
};

