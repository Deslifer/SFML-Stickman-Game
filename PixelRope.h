#pragma once
#include "Pixel.h"
class PixelRope :
	public Pixel
{
private:
	b2RopeJointDef rDef;
public:
	PixelRope(b2World* world, sf::Sprite sprite, float x, float y, b2BodyType type, float size, b2Body* body2,float size2, float originalScale);
	virtual ~PixelRope();

	void update(const float& dt);
};

