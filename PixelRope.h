#pragma once
#include "Pixel.h"
class PixelRope :
	public Pixel
{
private:
	b2RopeJointDef rDef;
public:
	PixelRope(b2World* world, float x, float y, b2BodyType type, float size, b2Body* body2);
	virtual ~PixelRope();

	void update(const float& dt);
};

