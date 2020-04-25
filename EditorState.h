#pragma once

#include "State.h"
#include "PlayerBox2D.h"
#include "Button.h"
#include "MyContactListener.h"

enum pixel_States { PIX_KINETIC = 0, PIX_DYNAMIC};
enum pixel_Types { PIX_POINT = 0, PIX_ROPE, PIX_SWORD, PIX_SHIELD};

class EditorState :
	public State, public b2ContactListener
{
private:
	//Variables
	PlayerBox2D* player;
	sf::Font font;
	std::map<std::string, Button*> buttons;

	sf::Texture backgroundTexture;
	sf::RectangleShape background;

	sf::Texture backgroundTexture1;
	sf::RectangleShape background1;

	sf::Texture boxTexture;
	sf::Sprite box;

	sf::Texture platformTexture;
	sf::Sprite platform;

	const float SCALE = 30.f;
	const float DEG = 57.29577f;
	std::unique_ptr<b2World> world;

	std::vector<b2Body*> boxes;
	std::vector<sf::Color> boxesColors;

	std::vector<b2Body*> bodyboxes;

	std::vector<b2Body*> platforms;
	std::vector<sf::Color> platformsColors;

	b2Fixture* m_platformFixture;


	int pixState = 0;
	int pixType = 0;
	bool popfront = 1;
	bool isWay = 0;

	//Functions
	

	void setWall(int x, int y, int w, int h);
	void setPlatform(float x, float y);
	void setPixel(float x, float y);
	void deletePixels();
	void initVariables();
	void initBackground();
	void initTextures();
	void initPlayers();
	void initFonts();
	void initKeybinds();
	void initButtons();
	void renderBoxes(sf::RenderTarget* target);
	void updatePlatforms();
	void renderPlatforms(sf::RenderTarget* target);
	void renderText(std::string s, float x, float y, unsigned int size, sf::RenderTarget* target);

public:
	EditorState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~EditorState();

	//Functions
	void updateEvents(sf::Event& event, const float& dt);
	void updateButtons();
	void updateInput(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target = NULL);
	void renderButtons(sf::RenderTarget* target = NULL);
};

