#include "SandboxState.h"
#include <random>
std::default_random_engine generator1;
std::uniform_int_distribution<int> distribution1(0, 255);

void SandboxState::setWall(int x, int y, int w, int h)
{
	b2PolygonShape gr;
	gr.SetAsBox(w / SCALE, h / SCALE);

	//Filter
	b2Filter f;
	f.categoryBits = CATEGORY_BODY;
	f.maskBits = MASK_BODY;
	//Filter

	b2BodyDef bdef;
	bdef.position.Set(x / SCALE, y / SCALE);
	bdef.type = b2_staticBody;
	b2Body* b_ground = world->CreateBody(&bdef);
	b_ground->CreateFixture(&gr, 0)->SetFilterData(f);
	b_ground->SetUserData((void*)"wall");
	b2Vec2 pos = b_ground->GetPosition();
	Ground = b_ground;
}

void SandboxState::initVariables()
{
	firstJoint = 1;
	pixScale = 1;
	pixState = 0;
	pixType = 1;
	popfront = 1;
	isWay = 0;
}

void SandboxState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);
	if (!this->backgroundTexture.loadFromFile("Sprites/BG.jpg"))
	{
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}
	//this->background.setFillColor(sf::Color::White);
	this->backgroundTexture.setSmooth(true);
	this->background.setTexture(&this->backgroundTexture);


	this->helpTexture.loadFromFile("Sprites/pixelTypes.png");
	this->helpSprite.setTexture(this->helpTexture);
	this->helpSprite.setPosition(this->window->getSize().x / 2 - this->helpTexture.getSize().x - 10,0);

}

void SandboxState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Sprites/Stickman/Sheet.png")) //Stickman.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}

}

void SandboxState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->font);

	this->pmenu->addButton("QUIT", 300.f, "Quit");
}

void SandboxState::initPlayers()
{
	this->player = new PlayerBox2D(this->world.get(), this->window->getSize().x / 2, this->window->getSize().y / 2, this->textures["PLAYER_SHEET"], 8, 10);
}

void SandboxState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/THANK YOU KOBE.ttf"))
	{
		throw("ERROR::SandboxState::COULD NOT LOAD FONT");
	}
}

void SandboxState::initKeybinds()
{
	std::ifstream fin("Config/SandboxStateKeyBinds.ini");
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

SandboxState::SandboxState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	:State(window, supportedKeys, states)
{
	b2Vec2 gravity(0.f, 9.8f);
	this->world = std::make_unique<b2World>(gravity);

	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initTextures();
	this->initPauseMenu();
	this->initPlayers();
	this->initKeybinds();

	pixels = new PixelControl(this->world.get(), this->player->getBody(), this->window->getSize().x, this->window->getSize().y,0);
	//setWall(-50, this->window->getSize().y / 2, 1, this->window->getSize().y / 2);
	//setWall(this->window->getSize().x + 50, this->window->getSize().y / 2, 1, this->window->getSize().y / 2);
	setWall(this->window->getSize().x / 2, this->window->getSize().y+10, this->window->getSize().x*2.f, 10);
}

SandboxState::~SandboxState()
{
	delete this->pmenu;

}

void SandboxState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("QUIT"))
		this->endState();
}

void SandboxState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime())
	{
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
		//this->endState();
	}
}

void SandboxState::updateEvents(sf::Event& event, const float& dt)
{

	if (event.type == sf::Event::MouseWheelMoved)
	{
		if (event.mouseWheel.delta == 1)
		{
			if (pixScale < 100)
				this->pixScale += 1;

		}
		else if (event.mouseWheel.delta == -1)
		{
			if (pixScale > 1)
				this->pixScale -= 1;
		}
		this->pixels->setScale(pixScale);
	}
	if (event.type == sf::Event::LostFocus)
		this->pauseState();

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Num1)
		{
			pixType = 1;
		}
		if (event.key.code == sf::Keyboard::Num2)
		{
			pixType = 2;
		}
		if (event.key.code == sf::Keyboard::Num3)
		{
			pixType = 3;
		}
		if (event.key.code == sf::Keyboard::Num4)
		{
			pixType = 4;
		}
		if (event.key.code == sf::Keyboard::Num5)
		{
			pixType = 5;
		}
		if (event.key.code == sf::Keyboard::Num6)
		{
			pixType = 6;
		}
		if (event.key.code == sf::Keyboard::Num7)
		{
			pixType = 7;
		}

		if (event.key.code == sf::Keyboard::Q)
		{
			if (pixState < PIX_DYNAMIC)
				pixState++;
			else
				pixState = PIX_KINETIC;
		}
		if (event.key.code == sf::Keyboard::F)
		{
			this->player->getSprite()->setColor(sf::Color(distribution1(generator1), distribution1(generator1), distribution1(generator1)));
		}
		if (event.key.code == sf::Keyboard::Tab)
		{
			if (isWay)
				isWay = false;
			else
				isWay = true;
		}
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Button::Left && pixType == PIX_BODY && pixState == PIX_DYNAMIC)
		{
			this->pixels->createBody();
		}
		if (event.mouseButton.button == sf::Mouse::Button::Left && pixType == PIX_MOUSE)
		{
			this->pixels->mouseJoint(this->mousePosView.x, this->mousePosView.y, firstJoint, true, Ground);
			firstJoint = 1;
		}
	}


}

void SandboxState::updatePlayerInput(const float& dt)
{
	//Update player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
	{

		this->player->move(-1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
	{
		this->player->move(1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))) && this->getKeyTime())
	{
		this->player->move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))) && this->getKeyTime())
	{
		this->player->move(0.f, 1.f, dt);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (pixType != PIX_JOINT && pixType != PIX_MOUSE)
			this->pixels->newPixel(this->mousePosView.x, this->mousePosView.y);
		else if (pixType != PIX_MOUSE)
		{
			if (this->getKeyTime())
			{
				this->pixels->joint(this->mousePosView.x, this->mousePosView.y, firstJoint);
				if (firstJoint)
					firstJoint = 0;
				else
					firstJoint = 1;
			}
		}
		else
		{
			this->pixels->mouseJoint(this->mousePosView.x, this->mousePosView.y, firstJoint, false, Ground);
			if (firstJoint)
				firstJoint = 0;
		}

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) && this->getKeyTime())
	{
		if (popfront)
			popfront = 0;
		else
			popfront = 1;

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		this->pixels->deletePixel();

	}

}

void SandboxState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateKeyTime(dt);
	this->updateInput(dt);

	if (!this->paused) //Unpaused update
	{
		world->Step(1.0f / 60.0f, 6, 2);
		//world->Step(1.0f / 60.0f, 1, 10);
		
		this->pixels->update(dt, isWay, popfront, pixState, pixType);
		this->updatePlayerInput(dt);
		this->player->update(dt);
	}
	else //Paused update
	{
		this->pmenu->update(this->mousePosView);
		this->updatePauseMenuButtons();
	}
}

void SandboxState::render(sf::RenderTarget* target)
{
	

	if (!target)
	{
		target = this->window;
	}

	target->draw(this->background);
	this->pixels->render(target);
	this->player->render(target);
	
	

	if (!this->paused)
	{
		if (pixType != PIX_MOUSE && pixType != PIX_JOINT)
			this->pixels->renderPixelHelper(target, mousePosView.x, mousePosView.y);
		if (popfront) {
			renderText("remove from end", 0, 0, 24, target);
		}
		else
		{
			renderText("remove from front", 0, 0, 24, target);
		}
		switch (pixType) {
		case PIX_POINT:
			renderText("Point", 0, 20, 24, target);
			break;
		case PIX_ROPE:
			renderText("Rope", 0, 20, 24, target);
			break;
		case PIX_BODY:
			renderText("Body", 0, 20, 24, target);
			break;
		case PIX_SHIELD:
			renderText("Shield", 0, 20, 24, target);
			break;
		case PIX_SWORD:
			renderText("Sword", 0, 20, 24, target);
			break;
		case PIX_MOUSE:
			renderText("Mouse", 0, 20, 24, target);
			break;
		case PIX_JOINT:
			renderText("Joint", 0, 20, 24, target);
			break;
		}

		switch (pixState) {
		case PIX_KINETIC:
			renderText("Static state", 0, 40, 24, target);
			break;
		case PIX_DYNAMIC:
			renderText("Dynamic state", 0, 40, 24, target);
			break;
		}
		if (isWay)
		{
			renderText("Road mode", 0, 60, 24, target);
		}
		else {
			renderText("Normal mode", 0, 60, 24, target);
		}
		renderText("Pixel size: ", 0, 80, 24, target);
		renderNumbers(pixScale, 110, 80, 24, target);
		target->draw(this->helpSprite);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		{
			renderText("Delete", this->mousePosView.x, this->mousePosView.y - 20, 12, target);
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
		{
			renderText("Create", this->mousePosView.x, this->mousePosView.y - 20, 12, target);
		}
	}
	else
	{
		this->pmenu->render(*target);
	}
}

void SandboxState::renderText(std::string s, float x, float y, unsigned int size, sf::RenderTarget* target)
{
	sf::Text mouseText;
	mouseText.setPosition(x, y);
	mouseText.setFont(this->font);
	mouseText.setFillColor(sf::Color::Black);
	mouseText.setCharacterSize(size);
	std::stringstream ss;
	ss << s;
	mouseText.setString(ss.str());
	target->draw(mouseText);
}

void SandboxState::renderNumbers(float s, float x, float y, unsigned int size, sf::RenderTarget* target)
{
	sf::Text mouseText;
	mouseText.setPosition(x, y);
	mouseText.setFont(this->font);
	mouseText.setFillColor(sf::Color::Black);
	mouseText.setCharacterSize(size);
	std::stringstream ss;
	ss << s;
	mouseText.setString(ss.str());
	target->draw(mouseText);
}
