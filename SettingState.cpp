#include "SettingState.h"

void SettingState::initVariables()
{
}

void SettingState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);
	if (!this->backgroundTexture.loadFromFile("Sprites/BGMenu.jpg"))
	{
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}

	this->background.setTexture(&this->backgroundTexture);
}

void SettingState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/THANK YOU KOBE.ttf"));
	{
		//throw("ERROR::SettingState::COULD NOT LOAD FONT");
	}
}

void SettingState::initKeybinds()
{
	std::ifstream fin("Config/SettingStateKeyBinds.ini");
	if (fin.is_open())
	{
		std::string key = "";
		std::string key2 = "";

		while (fin >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}
}

void SettingState::initButtons()
{
	
	this->buttons["EXIT_STATE"] = new Button(
		400, 400, 150, 50,
		&this->font, "Quit", 50,
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 250), sf::Color(20, 20, 20, 50),
		sf::Color(100, 100, 100, 0), sf::Color(150, 150, 150, 0), sf::Color(20, 20, 20, 0));

}

SettingState::SettingState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	:State(window, supportedKeys, states)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();
}

SettingState::~SettingState()
{
	auto it = this->buttons.begin();
	for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

//Accessors

//Functions
void SettingState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}
	//Quit
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->endState();
	}
}

void SettingState::updateEvents(sf::Event& event, const float& dt)
{
}

void SettingState::updateInput(const float& dt)
{
}

void SettingState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateButtons();

}

void SettingState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	target->draw(this->background);

	this->renderButtons(target);
}

void SettingState::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(*target);
	}
}


