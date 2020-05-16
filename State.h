#pragma once
#include "PlayerBox2D.h"

class State
{
private:

protected:
	AudioComponent* audioComponent;

	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool quit;
	bool paused;
	float keyTime;
	float keyTimeMax;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	std::map<std::string,sf::Texture> textures;

	//Functions
	virtual void initKeybinds() = 0;
public:
	State(sf::RenderWindow* window, std::map<std::string,int>* supportedKeys, std::stack<State*>* states);
	virtual ~State();
	
	
	const bool& getQuit() const;
	const bool getKeyTime();

	void endState();
	void pauseState();
	void unpauseState();

	virtual void updateEvents(sf::Event& event, const float& dt) = 0;
	virtual void updateKeyTime(const float& dt);
	virtual void updateMousePositions();
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target=nullptr) = 0;
};

