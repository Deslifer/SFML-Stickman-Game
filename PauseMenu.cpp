#include "PauseMenu.h"

PauseMenu::PauseMenu(sf::RenderWindow& window, sf::Font& font)
	:font(font)
{
	//Init background
	this->background.setSize(
		sf::Vector2f(
			static_cast<float>(window.getSize().x), 
			static_cast<float>(window.getSize().y)
		)
	);
	this->background.setFillColor(sf::Color(20, 20, 20, 100));

	//Init container
	this->container.setSize(
		sf::Vector2f(
			static_cast<float>(window.getSize().x) / 4.f,
			static_cast<float>(window.getSize().y) - 60.f
		)
	);

	this->container.setFillColor(sf::Color(20, 20, 20, 200));
	this->container.setPosition(
		static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
		30.f
	);

	//Init text
	this->menuText.setFont(font);
	this->menuText.setFillColor(sf::Color(255, 255, 255, 200));
	this->menuText.setCharacterSize(30.f);
	this->menuText.setString("PAUSED");
	this->menuText.setPosition(this->container.getPosition());

}

PauseMenu::~PauseMenu()
{
	auto it = this->buttons.begin();
	for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
}

std::map<std::string, Button*>& PauseMenu::getButtons()
{
	// TODO: вставьте здесь оператор return
	return this->buttons;
}

//Functions

const bool PauseMenu::isButtonPressed(const std::string key)
{
	// TODO: вставьте здесь оператор return
	return this->buttons[key]->isPressed();
}

void PauseMenu::addButton(const std::string key, float y, const std::string text)
{
	float width = 150.f;
	float height = 50.f;
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;
	this->buttons[key] = new Button(
		x, y, width, height,
		&this->font, text, 50.f,
		sf::Color(70.f, 70.f, 70.f, 200.f), sf::Color(150.f, 150.f, 150.f, 250.f), sf::Color(20.f, 20.f, 20.f, 50.f),
		sf::Color(70.f, 70.f, 70.f, 0.f), sf::Color(150.f, 150.f, 150.f, 0.f), sf::Color(20.f, 20.f, 20.f, 0.f));

}

void PauseMenu::update(const sf::Vector2f& mousePos)
{
	for (auto& i : this->buttons)
	{
		i.second->update(mousePos);
	}
}

void PauseMenu::render(sf::RenderTarget & target)
{
	target.draw(background);
	target.draw(container);

	target.draw(this->menuText);

	for (auto &i : this->buttons)
	{
		i.second->render(target);
	}

}
