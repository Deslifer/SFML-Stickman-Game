#pragma once
#include "PixelRope.h"
#include "PixelPoint.h"
#include "PixelBody.h"
#include "PixelSword.h"
#include "PixelShield.h"
#include <sstream>
#include <map>

enum PixelControl_States { PIX_KINETIC = 0, PIX_DYNAMIC };
enum PixelControl_Types { PIX_POINT = 1, PIX_ROPE, PIX_BODY, PIX_SWORD, PIX_SHIELD,PIX_MOUSE, PIX_JOINT };

class PixelControl
{
private:
	b2World* world = nullptr;
	b2Body* player = nullptr;
	b2BodyType type = b2_staticBody;

	sf::Font font;

	std::vector<b2Body*> newBodies;
	std::map<int,b2Fixture*> newFixtures;

	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<Pixel*> pixels;
	std::vector<float> pixelsTimer;

	Pixel* bodyFirstJoint = NULL;
	Pixel* bodySecondJoint = NULL;

	b2Body* movedBody = NULL;
	b2MouseJointDef mDef;
	b2MouseJoint* mjoint=NULL;
	

	float originScale;
	long firstpoint;
	bool isWay, popfront;
	int pixState, pixType;

	const bool withMaxPixels;
	int maxTimer;
	int maxPixels;
	float windowX,windowY;

public:
	PixelControl(b2World* world,b2Body* player,float windowX,float windowY,bool withMaxPixels);
	virtual ~PixelControl();
	void newPixel(float x, float y);
	void deletePixel();
	void joint(float x, float y,bool firstJoint);
	void mouseJoint(float x, float y, bool first, bool released, b2Body* ground);
	void createBody();
	void renderPixelHelper(sf::RenderTarget* target,float x,float y);
	b2Body* figure();
	void setScale(float scale);
	void addPixel();
	int getMaxPixels();
	void update(const float& dt, bool isWay, bool popfront, int pixState,int pixType);
	void render(sf::RenderTarget* target);
};

