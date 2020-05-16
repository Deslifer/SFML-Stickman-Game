#pragma once

#include "State.h"
#include "PlayerBox2D.h"
#include "Button.h"
#include "MyContactListener.h"
#include "PauseMenu.h"
#include "PixelControl.h"


class GameState :
	public State, public b2ContactListener
{
private:

	PauseMenu* pmenu;
	PixelControl* pixels;
	//Variables
	PlayerBox2D* player;
	sf::Font font;

	sf::Texture helpTexture;
	sf::Sprite helpSprite;

	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Texture backgroundTexture2;
	sf::RectangleShape background2;

	sf::Texture backgroundTexture1;
	sf::RectangleShape background1;

	sf::Texture platformTexture;
	sf::Sprite platform;

	std::unique_ptr<b2World> world;

	std::vector<b2Body*> platforms;
	std::vector<sf::Color> platformsColors;

	b2Fixture* m_platformFixture;
	float time;
	float platformSpeed;
	int SCORE;
	int nextSCORE;

	bool firstJoint;

	int pixScale;
	int pixState;
	int pixType;
	bool popfront;
	bool isWay;

	//Functions
	void setWall(int x, int y, int w, int h);
	void setPlatform(float x, float y);
	void initVariables();
	void initBackground();
	void initTextures();
	void initPauseMenu();
	void initPlayers();
	void initFonts();
	void initKeybinds();
	void updatePlatforms();
	void renderPlatforms(sf::RenderTarget* target);
	void renderText(std::string s, float x, float y, unsigned int size, sf::RenderTarget* target);
	void renderNumbers(float s, float x, float y, unsigned int size, sf::RenderTarget* target);

public:
	GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~GameState();

	//Functions
	void updatePauseMenuButtons();
	void updateInput(const float& dt);
	void updateEvents(sf::Event& event, const float& dt);
	void updatePlayerInput(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = NULL);
};

