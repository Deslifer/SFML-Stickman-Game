#pragma once
#include "State.h"
#include "Collision.hpp"


class GameState :
	public State 
{
private:
	Player* player;
	sf::Texture texture;
	sf::Texture backgroundTexture;
	sf::RectangleShape background;


	//Functions
	void initBackground();
	void initKeybinds();
	void initTextures();
	void initPlayers();
	void initVariables();
public:
	GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states);
	virtual ~GameState();

	//Functions
	void updateEvents(sf::Event& event, const float& dt);
	void updateInput(const float& dt);
	void update(const float& dt);
	void render(sf::RenderTarget* target=nullptr);
	
};

