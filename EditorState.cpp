#include "EditorState.h"
#include <random>
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 255);

void EditorState::setWall(int x, int y, int w, int h)
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

void EditorState::setPlatform(float x, float y)
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
	
	shape.SetAsBox(75.f / SCALE, 1.f / SCALE);
	bdef.position.Set(x / SCALE, y / SCALE);
	body = this->world->CreateBody(&bdef);
	this->m_platformFixture = body->CreateFixture(&shape, 1);
	this->m_platformFixture->SetFilterData(f);
	this->m_platformFixture->SetUserData((void*)"platform");
	body->SetUserData((void*)"platform");
	//body->SetLinearVelocity(b2Vec2(-distributionSpeed(generator), 0));
	this->platforms.push_back(body);
	color = sf::Color(distribution(generator), distribution(generator), distribution(generator));
	this->platformsColors.push_back(color);
}

void EditorState::setPixel(float x, float y)
{
	b2BodyDef bdef;
	b2Filter f;
	b2CircleShape circle;
	b2PolygonShape polyg;
	b2Body* body;
	b2FixtureDef fdef;
	b2Fixture* bfix;
	b2Filter f2;

	if (pixState == PIX_DYNAMIC)
	{
		bdef.type = b2_dynamicBody;
	}
	else if (pixState == PIX_KINETIC)
	{
		bdef.type = b2_staticBody;
	}
	

	if ((pixType == PIX_SHIELD || pixType == PIX_SWORD) && pixState== PIX_DYNAMIC)
	{
		f2.categoryBits = CATEGORY_BODY;
		f2.maskBits = MASK_BODY_SHIELD;
	}
	else
	{
		//Filter
		f.categoryBits = CATEGORY_BODY;
		f.maskBits = MASK_BODY;
		//Filter
	}

	bdef.position.Set(x / SCALE, y / SCALE);
	circle.m_radius = 5 / SCALE;
	polyg.SetAsBox(5 / SCALE, 5/SCALE);
	body = this->world->CreateBody(&bdef);
	if ((pixType == PIX_SHIELD || pixType == PIX_SWORD) && pixState == PIX_DYNAMIC)
	{
		bfix = body->CreateFixture(&circle, 0.1);
	}
	else
	{
		bfix = body->CreateFixture(&circle, 6);
	}
	bfix->SetFilterData(f);
	bfix->SetUserData((void*)1);
	circle.m_radius = 10 / SCALE;
	fdef.shape = &circle;
	//Filter
	f2.categoryBits = CATEGORY_NOT_BODY;
	f2.maskBits = MASK_NOT_BODY;
	//Filter
	bfix = body->CreateFixture(&fdef);
	bfix->SetFilterData(f2);
	bfix->SetUserData((void*)1);


	body->SetUserData((void*)"box");

	this->boxes.push_back(body);
	sf::Color color = sf::Color(distribution(generator), distribution(generator), distribution(generator));
	this->boxesColors.push_back(color);

		if (this->boxes.size() > 1 && pixType == PIX_ROPE) {
			b2RopeJointDef rDef;
			rDef.bodyA = this->boxes[this->boxes.size() - 1];
			rDef.bodyB = this->boxes[this->boxes.size() - 2];
			rDef.collideConnected = true;
			rDef.maxLength = 5 / SCALE;
			rDef.localAnchorA.Set(0, -1.f / SCALE);
			rDef.localAnchorB.Set(0, 1.f / SCALE);
			this->world->CreateJoint(&rDef);

			/*b2WeldJointDef rDef;
			rDef.bodyA = this->boxes[this->boxes.size() - 1];
			rDef.bodyB = this->boxes[this->boxes.size() - 2];
			rDef.collideConnected = true;
			rDef.localAnchorA.Set(0, 5 / SCALE);
			rDef.localAnchorB.Set(0, -5 / SCALE);
			this->world->CreateJoint(&rDef);*/
		}
		else if (pixType == PIX_SHIELD && pixState==PIX_DYNAMIC)
		{
			b2MotorJointDef rDef;
			rDef.bodyA = this->boxes[this->boxes.size() - 1];
			rDef.bodyB = this->player->getBody();
			rDef.collideConnected = true;
			rDef.maxForce = 1;
			this->world->CreateJoint(&rDef);
		}
		else if (pixType == PIX_SWORD && pixState == PIX_DYNAMIC)
		{
			b2MotorJointDef rDef;
			rDef.bodyA = this->boxes[this->boxes.size() - 1];
			rDef.bodyB = this->player->getBody();
			rDef.collideConnected = true;
			rDef.maxForce = -1;
			this->world->CreateJoint(&rDef);
		}
}

void EditorState::deletePixels()
{
	if (this->boxes.size() > 0 && this->boxesColors.size() > 0)
	{
		if (this->popfront)
		{
			this->world->DestroyBody(this->boxes[0]);
			this->boxes[0]->SetUserData(NULL);
			this->boxes[0] = NULL;
			this->boxes.erase(this->boxes.begin());
			this->boxesColors.erase(this->boxesColors.begin());
		}
		else
		{
			this->world->DestroyBody(this->boxes[boxes.size() - 1]);
			this->boxes[boxes.size() - 1]->SetUserData(NULL);
			this->boxes[boxes.size() - 1] = NULL;
			this->boxes.pop_back();
			this->boxesColors.pop_back();
		}
	}
}

void EditorState::initVariables()
{
}

void EditorState::initBackground()
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
	this->background.setTexture(&this->backgroundTexture);
	/*this->background1.setFillColor(sf::Color::Green);
	this->background1.setSize(sf::Vector2f(this->window->getSize().x, 40.f));
	this->background1.setPosition(0.f, this->window->getSize().y-40.f);*/
	
	this->boxTexture.loadFromFile("Sprites/circle.png");
	this->box.setTexture(this->boxTexture);
	this->box.setOrigin(this->boxTexture.getSize().x / 2, this->boxTexture.getSize().y / 2);

	this->platformTexture.loadFromFile("Sprites/platform.png");
	this->platformTexture.setSmooth(true);
	this->platform.setTexture(this->platformTexture);
	this->platform.setOrigin(this->platformTexture.getSize().x / 2, this->platformTexture.getSize().y / 2-10);
}

void EditorState::initTextures()
{
	if (!this->textures["PLAYER_SHEET"].loadFromFile("Sprites/Stickman/Sheet.png")) //Stickman.png"))
	{
		throw "ERROR::GAME_STATE::COULD_NOT_LOAD_PLAYER_TEXTURE";
	}
}

void EditorState::initPlayers()
{
	this->player = new PlayerBox2D(this->world.get(),this->window->getSize().x / 2, this->window->getSize().y / 2, this->textures["PLAYER_SHEET"]);
}

void EditorState::initFonts()
{
	if (!this->font.loadFromFile("Fonts/THANK YOU KOBE.ttf"))
	{
		throw("ERROR::EDITORSTATE::COULD NOT LOAD FONT");
	}
}

void EditorState::initKeybinds()
{
	std::ifstream fin("Config/EditorStateKeyBinds.ini");
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

void EditorState::initButtons()
{
	
}

EditorState::EditorState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
	:State(window, supportedKeys, states)
{
	b2Vec2 gravity(0.f, 9.8f);
	this->world = std::make_unique<b2World>(gravity);


	setWall(this->window->getSize().x/2.f, this->window->getSize().y +20, this->window->getSize().x, 20);
	setWall(-50, this->window->getSize().y/2, 1, this->window->getSize().y / 2);
	setWall(this->window->getSize().x+50, this->window->getSize().y / 2, 1, this->window->getSize().y / 2);
	setWall(this->window->getSize().x/2, -50, this->window->getSize().x / 2, 1);

	setPlatform(400, 600);

	this->world->SetContactListener(new MyContactListener(m_platformFixture));

	this->initVariables();
	this->initBackground();
	this->initTextures();
	this->initPlayers();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();
}

EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it)
	{
		delete it->second;
	}
	this->boxes.clear();
	this->boxesColors.clear();

}

void EditorState::updateEvents(sf::Event& event, const float& dt)
{

	if (event.type == sf::Event::MouseWheelMoved)
	{
		if (event.mouseWheel.delta == 1)
		{

			popfront = 0;
		}
		else if (event.mouseWheel.delta == -1)
		{
			popfront = 1;
		}
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Q)
		{
			if (pixType < PIX_SHIELD)
				pixType++;
			else
				pixType = PIX_POINT;
		}
		if (event.key.code == sf::Keyboard::E)
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
		if (event.key.code == sf::Keyboard::Tab)
		{
			if (isWay)
				isWay = false;
			else
				isWay = true;
		}
		/*if (event.key.code == sf::Keyboard::Key(this->keybinds.at("MOVE_UP")))
		{
			this->player->move(0.f, -1.f, dt);
		}*/
	}

}

void EditorState::updateButtons()
{
	for (auto& it : this->buttons)
	{
		it.second->update(this->mousePosView);
	}

}

void EditorState::updateInput(const float& dt)
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
	{
		this->player->move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
	{
		this->player->move(0.f, 1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->endState();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{


		//

		//
		bool create = 1;
		b2Vec2 pos = b2Vec2(this->mousePosView.x / SCALE, this->mousePosView.y / SCALE);
		for (int i = 0; i < this->boxes.size(); i++) {
			for (b2Fixture* f = boxes[i]->GetFixtureList(); f != 0; f = f->GetNext()) {
				if (f->TestPoint(pos)) {
					create = 0;
					break;
				}
			}
			if (!create)
				break;
		}
		if (create)
		{
			if (dt < 0.01) {
				setPixel(this->mousePosView.x, this->mousePosView.y);
			}
		}

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
	{
		/*if (dt < 0.01) {
			setPixel(this->mousePosView.x, this->mousePosView.y);
		}*/

		b2BodyDef bdef;
		b2Filter f;
		b2CircleShape circle;
		b2PolygonShape polyg;
		b2Body* body;
		b2FixtureDef fdef;
		b2Fixture* bfix;
		b2Filter f2;
		bdef.type = b2_dynamicBody;
		//Filter
		f.categoryBits = CATEGORY_BODY;
		f.maskBits = MASK_BODY;
		//Filter
		bdef.position.Set(this->mousePosView.x / SCALE, this->mousePosView.y / SCALE);
		circle.m_radius = 5 / SCALE;
		polyg.SetAsBox(5 / SCALE, 5 / SCALE);
		body = this->world->CreateBody(&bdef);
		bfix = body->CreateFixture(&circle, 6);
		bfix->SetFilterData(f);
		bfix->SetUserData((void*)1);
		circle.m_radius = 10 / SCALE;
		fdef.shape = &circle;
		//Filter
		f2.categoryBits = CATEGORY_NOT_BODY;
		f2.maskBits = MASK_NOT_BODY;
		//Filter
		bfix = body->CreateFixture(&fdef);
		bfix->SetFilterData(f2);
		bfix->SetUserData((void*)1);
		body->SetUserData((void*)"box");
		this->bodyboxes.push_back(body);


		if (this->boxes.size() > 1 && pixType == PIX_ROPE) {
			b2WeldJointDef rDef;
			rDef.bodyA = this->bodyboxes[this->bodyboxes.size() - 1];
			rDef.bodyB = this->bodyboxes[this->bodyboxes.size() - 2];
			rDef.collideConnected = true;
			rDef.localAnchorA.Set(0, 5 / SCALE);
			rDef.localAnchorB.Set(0, -5 / SCALE);
			this->world->CreateJoint(&rDef);
		}

	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		deletePixels();
	}

}

void EditorState::update(const float& dt)
{

	world->Step(1.0f / 60.0f, 6, 2);
	
	
	this->updateMousePositions();
	this->updateInput(dt);
	this->updateButtons();
	this->updatePlatforms();

	this->player->update(dt);

	if (dt > 0.01 || (isWay && boxes.size()>50))
	{
		this->deletePixels();
	}
	
	
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	target->draw(this->background);
	//target->draw(this->background1);

	this->renderButtons(target);

	this->renderPlatforms(target);
	this->renderBoxes(target);
	this->player->render(target);
	if (popfront) {
		renderText("remove from end",0,0,24,target);
	}
	else
	{
		renderText("remove from front", 0, 0, 24,target);
	}
	switch (pixType) {
	case PIX_POINT:
		renderText("Point", 0, 20,24, target);
		break;
	case PIX_ROPE:
		renderText("Rope", 0, 20,24, target);
		break;
	case PIX_SHIELD:
		renderText("Shield", 0, 20, 24,target);
		break;
	case PIX_SWORD:
		renderText("Sword", 0, 20, 24, target);
		break;
	}

	switch (pixState) {
	case PIX_KINETIC:
		renderText("Static state", 0, 40,24, target);
		break;
	case PIX_DYNAMIC:
		renderText("Dynamic state", 0, 40,24, target);
		break;
	}
	if(isWay)
	{
		renderText("Road mode", 0, 60, 24, target);
	}
	else {
		renderText("Normal mode", 0, 60, 24, target);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		//this->mouseText("Delete", target);
		renderText("Delete", this->mousePosView.x, this->mousePosView.y-20, 12, target);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)|| sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
	{
		//this->mouseText("Create", target);
		renderText("Create", this->mousePosView.x, this->mousePosView.y - 20, 12, target);
	}


}

void EditorState::renderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
	{
		it.second->render(target);
	}
}

void EditorState::renderBoxes(sf::RenderTarget* target)
{
	for (int i = 0; i < this->boxes.size(); i++) {
		b2Vec2 pos = this->boxes[i]->GetPosition();
		box.setColor(this->boxesColors[i]);
		
		box.setPosition(pos.x * SCALE, pos.y * SCALE);
		target->draw(box);
		if (this->boxes[i]->GetPosition().x * SCALE > this->window->getSize().x ||
			this->boxes[i]->GetPosition().x * SCALE < 0 ||
			this->boxes[i]->GetPosition().y * SCALE > this->window->getSize().y ||
			this->boxes[i]->GetPosition().y * SCALE < 0)
		{
			this->world->DestroyBody(this->boxes[i]);
			this->boxes[i]->SetUserData(NULL);
			this->boxes[i] = NULL;
			this->boxes.erase(this->boxes.begin() + i);
			this->boxesColors.erase(this->boxesColors.begin() + i);
		}
	}

	for (int i = 0; i < this->bodyboxes.size(); i++) {
		b2Vec2 pos = this->bodyboxes[i]->GetPosition();
		box.setPosition(pos.x * SCALE, pos.y * SCALE);
		box.setColor(sf::Color::Black);
		target->draw(box);

		if (this->bodyboxes[i]->GetPosition().x * SCALE > this->window->getSize().x ||
			this->bodyboxes[i]->GetPosition().x * SCALE < 0 ||
			this->bodyboxes[i]->GetPosition().y * SCALE > this->window->getSize().y ||
			this->bodyboxes[i]->GetPosition().y * SCALE < 0)
		{
			this->world->DestroyBody(this->bodyboxes[i]);
			this->bodyboxes[i]->SetUserData(NULL);
			this->bodyboxes[i] = NULL;
			this->bodyboxes.erase(this->bodyboxes.begin() + i);
		}
	}


}

void EditorState::updatePlatforms()
{
	if (platforms.size() <= 10)
	{
		std::uniform_int_distribution<int> distributionX(1, this->window->getSize().x / 250);
		std::uniform_int_distribution<int> distributionY(1,this->window->getSize().y/ 250);

		setPlatform(this->window->getSize().x + distributionX(generator) * 250, distributionY(generator) * 250);

	}
}

void EditorState::renderPlatforms(sf::RenderTarget* target)
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
		}
	}
}

void EditorState::renderText(std::string s, float x, float y, unsigned int size, sf::RenderTarget* target)
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
