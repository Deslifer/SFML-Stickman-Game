#pragma once
#include "PixelRope.h"
#include "PixelPoint.h"
#include "PixelBody.h"
#include "PixelSword.h"
#include "PixelShield.h"
#include <map>

enum PixelControl_States { PIX_KINETIC = 0, PIX_DYNAMIC };
enum PixelControl_Types { PIX_POINT = 0, PIX_ROPE, PIX_BODY, PIX_SWORD, PIX_SHIELD };

class PixelControl
{
private:
	b2World* world = nullptr;
	b2Body* player = nullptr;
	b2BodyType type = b2_staticBody;

	std::vector<b2Body*> newBodies;
	std::map<int,b2Fixture*> newFixtures;

	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<Pixel*> pixels;
	long firstpoint;
	bool isWay, popfront;
	int pixState, pixType;
	float windowX,windowY;
	const float SCALE = 30.f;
	const float DEG = 57.29577f;
	//bool popfront;
	//bool isWay;

public:
	PixelControl(b2World* world,b2Body* player,float windowX,float windowY);
	virtual ~PixelControl();
	void newPixel(float x, float y);
	void deletePixel();
	void createBody();
	b2Body* figure();
	void setScale(float scale);
	void update(const float& dt, bool isWay, bool popfront, int pixState,int pixType);
	void render(sf::RenderTarget* target);
};

