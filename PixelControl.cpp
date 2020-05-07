#include "PixelControl.h"

PixelControl::PixelControl(b2World* world, b2Body* player, float windowX, float windowY)
	:world(world), player(player), windowX(windowX), windowY(windowY)
{
	firstpoint = 0;
	pixState = 0;
	pixType = 0;
	popfront = 1;
	isWay = 0;
	this->texture.loadFromFile("Sprites/circle.png");
	//this->texture.setSmooth(true);
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin(this->texture.getSize().x / 2, this->texture.getSize().y / 2);
}

PixelControl::~PixelControl()
{
	pixels.clear();
	newFixtures.clear();
	newBodies.clear();

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

	if (create)
	{
		if (pixType != PIX_BODY || pixState == PIX_KINETIC)
			firstpoint++;

		if (pixState == PIX_DYNAMIC && pixType != PIX_BODY)
		{
			type = b2_dynamicBody;
		}
		else if(pixState == PIX_KINETIC || pixType == PIX_BODY)
		{
			type = b2_staticBody;
		}
		
		if (pixType == PIX_ROPE && pixels.size() > 0)
			pixels.push_back(new PixelRope(this->world,this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, pixels.back()->getBody()));
		else if (pixType == PIX_BODY && pixState == PIX_DYNAMIC && this->pixels.size() > firstpoint)
			pixels.push_back(new PixelBody(this->world,this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, pixels.back()->getBody()));
		else if (pixType == PIX_SHIELD && pixState == PIX_DYNAMIC)
			pixels.push_back(new PixelShield(this->world, this->sprite, x, y, type, this->texture.getSize().x*this->sprite.getScale().x, this->player));
		else if (pixType == PIX_SWORD && pixState == PIX_DYNAMIC)
			pixels.push_back(new PixelSword(this->world, this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x, this->player));
		else
			pixels.push_back(new PixelPoint(this->world, this->sprite, x, y, type, this->texture.getSize().x * this->sprite.getScale().x));
		
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
			if (firstpoint != 0 && firstpoint != this->pixels.size())
				firstpoint = this->pixels.size();
		}
	}
}

void PixelControl::createBody()
{
	float centerX = 0, centerY = 0, count = 0;;
	if (this->pixels.size() > (firstpoint+1))
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
	this->sprite.setScale((texture.getSize().x + scale) / texture.getSize().x, (texture.getSize().x + scale) / texture.getSize().x);
}

void PixelControl::update(const float& dt, bool isWay, bool popfront, int pixState, int pixType)
{
	//std::cout << this->world->GetBodyCount()<<std::endl;
	//std::cout << this->newFixtures.size() << std::endl;
	//std::cout << this->newBodies.size() << std::endl;
	if (firstpoint < 0)
		firstpoint = 0;
	this->isWay = isWay;
	this->popfront = popfront;
	this->pixState = pixState;
	this->pixType = pixType;

	if (dt > 0.01 || (this->isWay && pixels.size() > 30))
	{
		this->deletePixel();

	}

	

}

void PixelControl::render(sf::RenderTarget* target)
{
	for (int i = 0; i < this->pixels.size(); i++) 
	{
		pixels[i]->render(target);
		if (this->pixels[i]->getBody()->GetPosition().x * SCALE > this->windowX + pixels[i]->getSize() ||
			this->pixels[i]->getBody()->GetPosition().x * SCALE < 0-pixels[i]->getSize() ||
			this->pixels[i]->getBody()->GetPosition().y * SCALE > this->windowY + pixels[i]->getSize())
		{

			this->world->DestroyBody(this->pixels[i]->getBody());
			this->pixels[i]->getBody()->SetUserData(NULL);
			this->pixels[i] = NULL;
			this->pixels.erase(this->pixels.begin() + i);	
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
