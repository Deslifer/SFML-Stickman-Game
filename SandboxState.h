#pragma once

#include "State.h"
#include "PlayerBox2D.h"
#include "Button.h"
#include "MyContactListener.h"
#include "PauseMenu.h"
#include "PixelControl.h"
#include "PixelPoint.h"


class SandboxState :
	public State, public b2ContactListener
{
private:
	PauseMenu* pmenu;
	PixelControl* pixels;
	//Variables
	PlayerBox2D* player;
	sf::Font font;
	sf::Sprite helpSprite;
	sf::Texture helpTexture;

	sf::Texture backgroundTexture;
	sf::RectangleShape background;


	std::unique_ptr<b2World> world;

	bool firstJoint;
	b2Body* Ground;

	int pixScale;
	int pixState;
	int pixType;
	bool popfront;
	bool isWay;

	//Functions
	void setWall(int x, int y, int w, int h);
	void initVariables();
	void initBackground();
	void initTextures();
	void initPauseMenu();
	void initPlayers();
	void initFonts();
	void initKeybinds();
	void renderText(std::string s, float x, float y, unsigned int size, sf::RenderTarget* target);
	void renderNumbers(float s, float x, float y, unsigned int size, sf::RenderTarget* target);

public:
	SandboxState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~SandboxState();

	//Functions
	void updatePauseMenuButtons();
	void updateInput(const float& dt);
	void updateEvents(sf::Event& event, const float& dt);
	void updatePlayerInput(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = NULL);
};

