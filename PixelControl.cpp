#include "PixelControl.h"

PixelControl::PixelControl(b2World* world, b2Body* player, float windowX, float windowY, bool withMaxPixels)
	:world(world), player(player), windowX(windowX), windowY(windowY),withMaxPixels(withMaxPixels)
{
	originScale = 0.01;
	maxPixels = 30;
	maxTimer = 2;
	firstpoint = 0;
	pixState = 0;
	pixType = 0;
	popfront = 1;
	isWay = 0;
	if (!this->font.loadFromFile("Fonts/THANK YOU KOBE.ttf"))
	{
		throw("ERROR::GameState::COULD NOT LOAD FONT");
	}
	this->texture.loadFromFile("Sprites/circleBig.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(originScale, originScale);
	this->sprite.setOrigin(this->texture.getSize().x / 2, this->texture.getSize().y / 2);
}

PixelControl::~PixelControl()
{
	pixels.clear();
	newFixtures.clear();
	newBodies.clear();
	pixelsTimer.clear();

}

void PixelControl::newPixel(float x, float y)
{
	bool create = 1;
	b2Vec2 pos = b2Vec2(x / SCALE, y / SCALE);
	for (int i = 0; i < this->pixels.size(); i++) {
		for (b2Fixture* f = pixels[i]->getBody()->GetFixtureList(); f != 0; f = f->GetNext()) {
			if (f->TestPoint(pos)) {
				create = 0;
				break;
			}
		}
		if (!create)
			break;
	}

	if (create && ((maxPixels>0 && withMaxPixels && maxPixels>=(sprite.getScale().x / originScale)) || !withMaxPixels))
	{
		if (maxPixels > 0 && withMaxPixels)
		{
			maxPixels -= sprite.getScale().x / originScale;
		}
		if (pixType != PIX_BODY || pixState == PIX_KINETIC)
			firstpoint++;

		if (pixState == PIX_DYNAMIC && pixType != PIX_BODY)
		{
			type = b2_dynamicBody;
		}
		else if(pixState == PIX_KINETIC || pixType == PIX_BODY)
		{
			type = b2_kinematicBody;
		}
		
		if (pixType == PIX_ROPE && pixels.size() > 0)
			pixels.push_back(new PixelRope(this->world,this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, pixels.back()->getBody(),pixels.back()->getSize(), originScale));
		else if (pixType == PIX_BODY && pixState == PIX_DYNAMIC && this->pixels.size() > firstpoint)
			pixels.push_back(new PixelBody(this->world,this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, pixels.back()->getBody(), originScale));
		else if (pixType == PIX_SHIELD && pixState == PIX_DYNAMIC)
			pixels.push_back(new PixelShield(this->world, this->sprite, x, y, type, this->texture.getSize().x*this->sprite.getScale().x, this->player, originScale));
		else if (pixType == PIX_SWORD && pixState == PIX_DYNAMIC)
			pixels.push_back(new PixelSword(this->world, this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, this->player, originScale));
		else
			pixels.push_back(new PixelPoint(this->world, this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, originScale));
		if(withMaxPixels)
			pixelsTimer.push_back(maxTimer);

	}
}

void PixelControl::deletePixel()
{
	if (this->pixels.size() > 0)
	{
		if (popfront)
		{
			std::map<int, b2Fixture*>::iterator it = this->newFixtures.find(0);
			if (it != this->newFixtures.end()) {
				if (it->second != NULL && it->first != NULL)
				{
					b2Body* b = it->second->GetBody();
					if (b != NULL) {
						b->DestroyFixture(it->second);
						this->newFixtures.erase(it);
					}
					else
					{
						this->newFixtures.erase(it);
					}
				}
			}

			this->world->DestroyBody(this->pixels.front()->getBody());
			this->pixels.front()->getBody()->SetUserData(NULL);
			this->pixels.front() = NULL;
			this->pixels.erase(this->pixels.begin());
			if (withMaxPixels)
				this->pixelsTimer.erase(this->pixelsTimer.begin());



			if (firstpoint != 0)
				firstpoint--;
		}
		else
		{
			std::map<int, b2Fixture*>::iterator it = this->newFixtures.find(pixels.size() - 1);
			if (it != this->newFixtures.end()) {
				if (it->second != NULL && it->first != NULL)
				{
					b2Body* b = it->second->GetBody();
					if (b != NULL) {
						b->DestroyFixture(it->second);
						this->newFixtures.erase(it);
					}
					else
					{
						this->newFixtures.erase(it);
					}
				}
			}
			this->world->DestroyBody(this->pixels.back()->getBody());
			this->pixels.back()->getBody()->SetUserData(NULL);
			this->pixels.back() = NULL;
			this->pixels.pop_back();
			if (withMaxPixels)
				this->pixelsTimer.pop_back();
			if (firstpoint != 0 && firstpoint != this->pixels.size())
				firstpoint = this->pixels.size();
		}
		
	}
}

void PixelControl::joint(float x, float y, bool firstJoint)
{
	bool joint = 0;
	b2Vec2 pos = b2Vec2(x / SCALE, y / SCALE);
	for (int i = 0; i < this->pixels.size(); i++) {
		for (b2Fixture* f = pixels[i]->getBody()->GetFixtureList(); f != 0; f = f->GetNext()) {
			if (f->TestPoint(pos) && f->GetUserData()!= (void*)"pixelAura") {
				if (firstJoint)
					bodyFirstJoint = pixels[i];
				else
				{
					bodySecondJoint = pixels[i];
					joint = 1;
				}
				break;
			}
		}
	}
	if (joint && (bodyFirstJoint!= bodySecondJoint) && bodyFirstJoint->getBody()->GetWorld()==this->world&& bodySecondJoint->getBody()->GetWorld() == this->world) {
		b2RopeJointDef rDef;
		rDef.bodyA = bodyFirstJoint->getBody();
		rDef.bodyB = bodySecondJoint->getBody();
		rDef.collideConnected = true;
		if(bodySecondJoint->getSize()>bodyFirstJoint->getSize())
			rDef.maxLength = bodySecondJoint->getSize() / 2 / SCALE;
		else
			rDef.maxLength = bodyFirstJoint->getSize() / 2 / SCALE;
		rDef.localAnchorA.Set(0, -1 / SCALE);
		rDef.localAnchorB.Set(0, 1 / SCALE);
		//rDef.localAnchorB.Set((-rDef.bodyB->GetPosition().x+pos.x)/2, (-rDef.bodyB->GetPosition().y+pos.y)/2);
		world->CreateJoint(&rDef);
	}
}

void PixelControl::mouseJoint(float x, float y, bool first,bool released,b2Body* ground)
{
	if (released)
	{
		if (mjoint != NULL &&movedBody!=NULL && movedBody->GetWorld() == this->world)
		{
			this->world->DestroyJoint(mjoint);
			mjoint = NULL;
			movedBody = NULL;
		}
	}
	else
	{
		b2Vec2 pos = b2Vec2(x / SCALE, y / SCALE);
		if (first)
		{
			bool joint = 0;
			for (int i = 0; i < this->pixels.size(); i++) {
				for (b2Fixture* f = pixels[i]->getBody()->GetFixtureList(); f != 0; f = f->GetNext()) {
					if (f->TestPoint(pos) && f->GetUserData() != (void*)"pixelAura") {

						movedBody = pixels[i]->getBody();
						joint = 1;
						break;
					}
				}
			}
			if (joint )
			{
				if (movedBody->GetWorld() == this->world)
				{
					mDef.bodyA = ground;
					mDef.bodyB = movedBody;
					mDef.target.Set(x / SCALE, y / SCALE);
					mDef.collideConnected = true;
					mDef.maxForce = 10000.f * movedBody->GetMass();
					mDef.frequencyHz = 5;
					mDef.dampingRatio = 0.9;
					mjoint = (b2MouseJoint*)this->world->CreateJoint(&mDef);
				}
			}
		}
		else
		{
			if(mjoint!=NULL && movedBody->GetWorld() == this->world)
				mjoint->SetTarget(pos);
		}
	}
}

void PixelControl::createBody()
{
	float centerX = 0, centerY = 0, count = 0;;
	if (this->pixels.size()-1 > (firstpoint))
	{

		b2Body* centerPixel = figure();
		for (int i = firstpoint; i < this->pixels.size(); i++)
		{
			b2Vec2 worldCoordsAnchorPoint = this->pixels[i]->getBody()->GetWorldPoint(b2Vec2(0.6f, 0));
			b2WeldJointDef wDef;
			wDef.bodyA = centerPixel;
			wDef.bodyB = this->pixels[i]->getBody();
			wDef.localAnchorA = wDef.bodyA->GetLocalPoint(worldCoordsAnchorPoint);
			wDef.localAnchorB = wDef.bodyB->GetLocalPoint(worldCoordsAnchorPoint);
			wDef.referenceAngle = wDef.bodyB->GetAngle() - wDef.bodyA->GetAngle();
			this->world->CreateJoint(&wDef);
			this->pixels[i]->getBody()->SetType(b2_dynamicBody);
		}
		centerPixel->SetType(b2_dynamicBody);
		firstpoint = this->pixels.size();
	}
	else if (pixels.size() > 0)
	{
		this->pixels.back()->getBody()->SetType(b2_dynamicBody);
		firstpoint++;
	}
}

void PixelControl::renderPixelHelper(sf::RenderTarget* target,float x, float y)
{

	sprite.setColor(sf::Color(0, 0, 0, 100));
	sprite.setPosition(x, y);
	target->draw(sprite);
}

b2Body* PixelControl::figure()
{
	b2Fixture* newFix;
	b2Body* newBody;
	b2BodyDef newDef;
	b2CircleShape newShape;
	//Filter
	b2Filter f;
	f.categoryBits = CATEGORY_BODY;
	f.maskBits = MASK_BODY;
	//Filter
	newDef.type = b2_staticBody;
	float newX = pixels[firstpoint]->getBody()->GetPosition().x;
	float newY = pixels[firstpoint]->getBody()->GetPosition().y;
	newDef.position.Set(newX, newY);
	newBody = this->world->CreateBody(&newDef);
	newShape = pixels[firstpoint]->getShape();
	newFix = newBody->CreateFixture(&newShape, pixels[firstpoint]->getFixture()->GetDensity());
	newFix->SetFilterData(f);
	this->newFixtures.insert(std::pair<int, b2Fixture*>(firstpoint, newFix));

	for (int i = firstpoint+1; i < this->pixels.size(); i++)
	{
		float dx, dy;
		dx = pixels[firstpoint]->getBody()->GetPosition().x - pixels[i]->getBody()->GetPosition().x;
		dy = pixels[firstpoint]->getBody()->GetPosition().y - pixels[i]->getBody()->GetPosition().y;
		newShape = pixels[i]->getShape();
		newShape.m_p.Set(-dx, -dy);
		newFix = newBody->CreateFixture(&newShape, pixels[i]->getFixture()->GetDensity());
		newFix->SetFilterData(f);
		this->newFixtures.insert(std::pair<int, b2Fixture*>(i, newFix));
	}
	newBody->SetUserData((void*)"newBody");
	newBodies.push_back(newBody);
	return newBody;
}

void PixelControl::setScale(float scale)
{
	this->sprite.setScale(originScale*scale, originScale * scale);
}

void PixelControl::addPixel()
{
	maxPixels++;
}

int PixelControl::getMaxPixels()
{
	return maxPixels;
}

void PixelControl::update(const float& dt, bool isWay, bool popfront, int pixState, int pixType)
{
	//std::cout << this->world->GetBodyCount()<<std::endl;
	//std::cout << this->pixels.size() << std::endl;
	//std::cout << this->newFixtures.size() << std::endl;
	//std::cout << this->newBodies.size() << std::endl;
	if (firstpoint < 0)
		firstpoint = 0;
	this->isWay = isWay;
	this->popfront = popfront;
	this->pixState = pixState;
	this->pixType = pixType;

	if (withMaxPixels)
	{
		if (maxPixels < 0)
			maxPixels = 0;
		for (int i = 0; i < this->pixelsTimer.size(); i++)
		{
			pixelsTimer[i] -= dt;
			if (pixelsTimer[i] < 0)
			{
				this->world->DestroyBody(this->pixels[i]->getBody());
				this->pixels[i]->getBody()->SetUserData(NULL);
				this->pixels[i] = NULL;
				this->pixels.erase(this->pixels.begin() + i);
				this->pixelsTimer.erase(this->pixelsTimer.begin() + i);
				if (firstpoint != 0 && i < firstpoint)
					firstpoint--;

				std::map<int, b2Fixture*>::iterator it = this->newFixtures.find(i);
				if (it != this->newFixtures.end()) {
					if (it->second != NULL && it->first != NULL)
					{
						b2Body* b = it->second->GetBody();
						if (b != NULL) {
							b->DestroyFixture(it->second);
							this->newFixtures.erase(it);
						}
						else
						{
							this->newFixtures.erase(it);
						}
					}
				}
				i--;
			}
		}
	}

	if (/*dt > 0.012  || */(this->isWay && pixels.size() > 30))
	{
		this->deletePixel();

	}

	

}

void PixelControl::render(sf::RenderTarget* target)
{

	for (int i = 0; i < this->pixels.size(); i++) 
	{
		pixels[i]->render(target);
		if (withMaxPixels)
		{
			sf::Text timerText;
			timerText.setFont(this->font);
			timerText.setFillColor(sf::Color::Black);
			timerText.setCharacterSize(pixels[i]->getSize());
			timerText.setOrigin(pixels[i]->getSize()/4.5, pixels[i]->getSize()/1.5);
			timerText.setPosition(
				this->pixels[i]->getBody()->GetPosition().x * SCALE,
				this->pixels[i]->getBody()->GetPosition().y * SCALE
			);


			std::stringstream ss;
			ss << (int)pixelsTimer[i]+1;
			timerText.setString(ss.str());
			target->draw(timerText);

		}
		if (this->pixels[i]->getBody()->GetPosition().x * SCALE > this->windowX + pixels[i]->getSize() ||
			this->pixels[i]->getBody()->GetPosition().x * SCALE < 0-pixels[i]->getSize() ||
			this->pixels[i]->getBody()->GetPosition().y * SCALE > this->windowY + pixels[i]->getSize())
		{

			this->world->DestroyBody(this->pixels[i]->getBody());
			this->pixels[i]->getBody()->SetUserData(NULL);
			this->pixels[i] = NULL;
			this->pixels.erase(this->pixels.begin() + i);	
			if (withMaxPixels)
			{
				this->pixelsTimer.erase(this->pixelsTimer.begin() + i);
			}
			
			if (firstpoint != 0 && i < firstpoint)
				firstpoint--;

			std::map<int, b2Fixture*>::iterator it = this->newFixtures.find(i);
			if (it != this->newFixtures.end()) {
				if (it->second != NULL && it->first != NULL)
				{
					b2Body* b = it->second->GetBody();
					if (b != NULL) {
						b->DestroyFixture(it->second);
						this->newFixtures.erase(it);
					}
					else
					{
						this->newFixtures.erase(it);
					}
				}
			}
			i--;
		}
	}

	for (int i = 0; i < this->newBodies.size(); i++)
	{
		if (!newBodies[i]->GetJointList())
		{
			if (newFixtures.size() > 0) {
				auto it = this->newFixtures.begin();
				while (it != newFixtures.end())
				{
					if (it->second->GetBody() == newBodies[i])
					{
						newFixtures.erase(it);
						it = this->newFixtures.begin();
					}
					else
					{
						it++;
					}
				}
			}
			this->world->DestroyBody(this->newBodies[i]);
			this->newBodies[i]->SetUserData(NULL);
			this->newBodies[i] = NULL;
			this->newBodies.erase(this->newBodies.begin() + i);
		}
	}
}
