#pragma once
#include "Pixel.h"
class PixelPoint :
	public Pixel
{
private:
public:
	PixelPoint(b2World* world, float x, float y, b2BodyType type, float size);
	virtual ~PixelPoint();

	void update(const float& dt);
};

