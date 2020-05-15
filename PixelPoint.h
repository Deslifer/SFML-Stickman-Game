#pragma once
#include "Pixel.h"
class PixelPoint :
	public Pixel
{
private:
public:
	PixelPoint(b2World* world, sf::Sprite sprite, float x, float y, b2BodyType type, float size, float originalScale);
	virtual ~PixelPoint();

	void update(const float& dt);
};

