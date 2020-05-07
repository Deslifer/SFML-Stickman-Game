#pragma once
#include"EntityBox2D.h"
#include"Box2D/box2d.h"


class PlayerBox2D :
	public EntityBox2D
{
private:
	//Variables
	//Initialazer functions;
	void initVariables();
	void initComponents();
	const float SCALE = 30.f;
	const float DEG = 57.29577f;
	


public:
	PlayerBox2D(b2World* world,float x, float y, sf::Texture& texture_sheet);
	virtual ~PlayerBox2D();

	//Functions
	virtual void update(const float& dt);
	sf::Sprite* getSprite();
	sf::Vector2f getPosition();
	b2Body* getBody();
};