#include "GameState.h"
#include <random>
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 255);

void GameState::setWall(int x, int y, int w, int h)
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
}

void GameState::setPlatform(float x, float y)
{
	std::uniform_int_distribution<int> distributionSpeed(1, 3);
	//Filter
	b2Filter f;
	b2BodyDef bdef;
	b2PolygonShape shape;
	b2Body* body;
	sf::Color color;
	f.categoryBits = CATEGORY_PLATFORM;
	f.maskBits = MASK_PLATFORM;
	//Filter
	bdef.type = b2_kinematicBody;
	shape.SetAsBox(75.f / SCALE, 8.f / SCALE);
	bdef.position.Set(x / SCALE, y / SCALE);
	body = this->world->CreateBody(&bdef);
	this->m_platformFixture = body->CreateFixture(&shape, 5);
	this->m_platformFixture->SetFilterData(f);
	this->m_platformFixture->SetUserData((void*)"platform");
	body->SetUserData((void*)"platform");
	body->SetLinearVelocity(b2Vec2(-this->platformSpeed, 0));
	this->platforms.push_back(body);
	color = sf::Color(distribution(generator), distribution(generator), distribution(generator));
	this->platformsColors.push_back(color);
}

void GameState::initVariables()
{
	this->firstJoint = 1;
	this->SCORE = 0;
	this->nextSCORE = 3;
	this->platformSpeed = 1;
	this->pixScale = 1;
	this->pixState = 0;
	this->pixType = 1;
	this->popfront = 1;
	this->isWay = 0;
	this->time = 0;
}

void GameState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(
			static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);
	this->background2.setSize(
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
	if (!this->backgroundTexture2.loadFromFile("Sprites/BGR.jpg"))
	{
		throw "ERROR::MAIN_MENU_STATE::FAILED_TO_LOAD_BACKGROUND_TEXTURE";
	}
	//this->background.setFillColor(sf::Color::White);
	this->backgroundTexture.setSmooth(true);
	this->backgroundTexture2.setSmooth(true);
	this->background.setTexture(&this->backgroundTexture);
	this->background2.setTexture(&this->backgroundTexture2);
	this->background2.setPosition(this->window->getSize().x - 1, 0.f);

	this->helpTexture.loadFromFile("Sprites/pixelTypes.png");
	this->helpSprite.setTexture(this->helpTexture);
	this->helpSprite.setTextureRect(sf::IntRect(0, 0, 171, 58));
	this->helpSprite.setPosition(this->window->getSize().x / 2 - this->helpTexture.getSize().x - 10, 0);

	this->platformTexture.loadFromFile("Sprites/platform.png");
	this->platformTexture.setSmooth(true);
	this->platform.setTexture(this->platformTexture);
	this->platform.setOrigin(this->platformTexture.getSize().x / 2, this->platformTexture.getSize().y / 2 - 10);
}

void GameState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Sprites/Stickman/Sheet.png")) //Stickman.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}
}

void GameState::initPauseMenu()
{
	this->pmenu = new PauseMenu(*this->window, this->font);

	this->pmenu->addButton("QUIT", 300.f, "Quit");
}

void GameState::initPlayers()
{
	this->player = new PlayerBox2D(this->world.get(), this->window->getSize().x / 2, this->window->getSize().y / 2, this->textures["PLAYER_SHEET"],8,10);
}

void GameState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/THANK YOU KOBE.ttf"))
	{
		throw("ERROR::GameState::COULD NOT LOAD FONT");
	}
}

void GameState::initKeybinds()
{
	std::ifstream fin("Config/GameStateKeyBinds.ini");
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

GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
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

	this->audioComponent = new AudioComponent();
	this->audioComponent->addMusic("MUSIC", "Audio/bensound-funkyelement.ogg");
	this->audioComponent->addSound("FALL_SOUND", "Audio/goofy-yell.ogg");

	pixels = new PixelControl(this->world.get(),this->player->getBody(),this->window->getSize().x, this->window->getSize().y,1);

	setPlatform(400, 600);

	this->world->SetContactListener(new MyContactListener(m_platformFixture));
}

GameState::~GameState()
{
	delete this->pmenu;
	delete this->audioComponent;
}

void GameState::updatePauseMenuButtons()
{
	if (this->pmenu->isButtonPressed("QUIT"))
		this->endState();
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime() )
	{
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
		//this->endState();
	}
}

void GameState::updateEvents(sf::Event& event, const float& dt)
{

	if (event.type == sf::Event::MouseWheelMoved)
	{
		if (event.mouseWheel.delta == 1)
		{
			if(pixScale<100)
			this->pixScale+=1;

		}
		else if (event.mouseWheel.delta == -1)
		{
			if(pixScale>1)
			this->pixScale-=1;
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
		if (event.key.code == sf::Keyboard::Q)
		{
			if (pixState < PIX_DYNAMIC)
				pixState++;
			else
				pixState = PIX_KINETIC;
		}
		if (event.key.code == sf::Keyboard::F)
		{
			this->player->getSprite()->setColor(sf::Color(distribution(generator), distribution(generator), distribution(generator)));
		}
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Button::Left && pixType == PIX_BODY && pixState == PIX_DYNAMIC)
		{
			this->pixels->createBody();
		}
	}


}

void GameState::updatePlayerInput(const float& dt)
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
		this->pixels->newPixel(this->mousePosView.x, this->mousePosView.y);
		
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle) && this->getKeyTime())
	{
		/*if (popfront)
			popfront = 0;
		else
			popfront = 1;*/

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		this->pixels->deletePixel();
		
	}

}

void GameState::update(const float& dt)
{

	if (states->size() > 0 && states->top() == this)
	{
		audioComponent->playMusic("MUSIC");
	}
	else
	{
		audioComponent->stopMusic("MUSIC");
	}

	this->updateMousePositions();
	this->updateKeyTime(dt);
	this->updateInput(dt);

	if (!this->paused) //Unpaused update
	{
		world->Step(1.0f / 60.0f, 6, 2);
		time += dt;
		if (time > 3)
		{
			SCORE++;
			this->pixels->addPixel();
			time = 0;
			if (SCORE == this->nextSCORE)
			{
				this->nextSCORE = SCORE + 10;
				if (this->platformSpeed < 7)
				{
					this->platformSpeed *= 1.5;
					for (auto* it : this->platforms)
					{
						it->SetLinearVelocity(b2Vec2(-this->platformSpeed, 0));
					}
				}
			}
		}
		this->pixels->update(dt, isWay, popfront, pixState, pixType);
		this->background.setPosition(this->background.getPosition().x - 0.1, this->background.getPosition().y);
		this->background2.setPosition(this->background2.getPosition().x - 0.1, this->background2.getPosition().y);
		if (background.getPosition().x + this->window->getSize().x - 1 < 0)
		{
			this->background.setPosition(this->window->getSize().x - 1, 0.f);
		}
		if (background2.getPosition().x + this->window->getSize().x - 1 < 0)
		{
			this->background2.setPosition(this->window->getSize().x - 1, 0.f);
		}
		this->updatePlayerInput(dt);
		this->updatePlatforms();
		this->player->update(dt);

		if (player->getPosition().y > window->getSize().y + 200)
		{
			this->audioComponent->playSound("FALL_SOUND");
			int highScore = 0;
			std::ifstream fin("Config/HighScore.ini");
			fin >> highScore;
			std::stringstream ss;
			if (SCORE > highScore)
			{
				ss << "WINNER SCORE: " << SCORE;
				this->pmenu->addText(200, ss.str());
				std::ofstream fout("Config/HighScore.ini");
				fout << SCORE;
			}
			else
			{
				ss << "LOSER SCORE: " << SCORE;
				this->pmenu->addText(200, ss.str());
			}
			this->pauseState();


		}
	}
	else //Paused update
	{
		this->pmenu->update(this->mousePosView);
		this->updatePauseMenuButtons();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	
	if (!target)
	{
		target = this->window;
	}

	target->draw(this->background);
	target->draw(this->background2);

	this->renderPlatforms(target);
	this->pixels->render(target);
	this->player->render(target);

	if (!this->paused)
	{
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

		renderText("Pixels left: ", 0, 100, 24, target);
		renderNumbers(this->pixels->getMaxPixels(), 120, 100, 24, target);

		renderText("SCORE: ", (this->window->getSize().x / 2), 0, 24, target);
		renderNumbers(SCORE, (this->window->getSize().x / 2) + 70, 0, 24, target);

		renderText("SPEED: ", (this->window->getSize().x / 2) + 200, 0, 24, target);
		renderNumbers(platformSpeed, (this->window->getSize().x / 2) + 270, 0, 24, target);

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

void GameState::updatePlatforms()
{
	if (platforms.size() <= 10)
	{
		std::uniform_int_distribution<int> distributionX(1, 3);
		std::uniform_int_distribution<int> distributionY(1, this->window->getSize().y / 200);

		float cox, coy;

		if (platforms.size() > 1)
		{
			setPlatform(platforms.back()->GetPosition().x * SCALE + 400, distributionY(generator) * 200);
			cox = platforms.back()->GetPosition().x * SCALE + 400;
			coy = distributionY(generator) * 200;
			while (coy < platforms.back()->GetPosition().y * SCALE + 10 && coy > platforms.back()->GetPosition().y* SCALE - 10)
			{
				coy = distributionY(generator) * 200;
			}
		}
		else
		{
			cox = this->window->getSize().x + 250;
			coy = distributionY(generator) * 200;
		}
		setPlatform(cox, coy);

	}
}

void GameState::renderPlatforms(sf::RenderTarget* target)
{
	for (int i = 0; i < this->platforms.size(); i++) {
		b2Vec2 pos = this->platforms[i]->GetPosition();
		platform.setColor(this->platformsColors[i]);
		platform.setPosition(pos.x * SCALE, pos.y * SCALE);
		target->draw(platform);
		if (this->platforms[i]->GetPosition().x * SCALE + platformTexture.getSize().x / 2 < 0)
		{
			this->world->DestroyBody(this->platforms[i]);
			this->platforms[i]->SetUserData(NULL);
			this->platforms[i] = NULL;
			this->platforms.erase(this->platforms.begin() + i);
			this->platformsColors.erase(this->platformsColors.begin() + i);
			i--;
		}
	}
}

void GameState::renderText(std::string s, float x, float y, unsigned int size, sf::RenderTarget* target)
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

void GameState::renderNumbers(float s, float x, float y, unsigned int size, sf::RenderTarget* target)
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
