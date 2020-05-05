#pragma once
#include "Pixel.h"
class PixelBody :
	public Pixel
{
private:
	b2WeldJointDef wDef;
public:
	PixelBody(b2World* world, float x, float y, b2BodyType type, float size, b2Body* body2);
	virtual ~PixelBody();

	void update(const float& dt);
};

