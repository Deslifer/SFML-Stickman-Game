#include "PixelControl.h"
#include <random>

std::default_random_engine generator1;
std::uniform_int_distribution<int> distribution1(0, 255);

PixelControl::PixelControl(b2World* world, b2Body* player, float windowX, float windowY)
	:world(world), player(player), windowX(windowX), windowY(windowY)
{
	firstpoint = 0;
	pixState = 0;
	pixType = 0;
	popfront = 1;
	isWay = 0;
	this->texture.loadFromFile("Sprites/circle.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin(this->texture.getSize().x / 2, this->texture.getSize().y / 2);
}

PixelControl::~PixelControl()
{
	for (auto pixel : this->pixels)
	{
		this->world->DestroyBody(pixel->getBody());
		pixel->getBody()->SetUserData(NULL);
		pixel = NULL;
		this->pixels.pop_back();
	}
	pixels.clear();
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
		
		if (pixType == PIX_ROPE && pixels.size() > 1)
			pixels.push_back(new PixelRope(this->world, x, y, type, this->texture.getSize().x, pixels.back()->getBody()));
		else if (pixType == PIX_BODY && pixState == PIX_DYNAMIC && this->pixels.size() > (firstpoint + 1))
			pixels.push_back(new PixelBody(this->world, x, y, type, this->texture.getSize().x, pixels.back()->getBody()));
		else if (pixType == PIX_SHIELD && pixState == PIX_DYNAMIC)
			pixels.push_back(new PixelShield(this->world, x, y, type, this->texture.getSize().x, this->player));
		else if (pixType == PIX_SWORD && pixState == PIX_DYNAMIC)
			pixels.push_back(new PixelSword(this->world, x, y, type, this->texture.getSize().x, this->player));
		else
			pixels.push_back(new PixelPoint(this->world, x, y, type, this->texture.getSize().x));
		
	}
}

void PixelControl::deletePixel()
{
	if (this->pixels.size() > 0)
	{
		if (popfront)
		{
			this->world->DestroyBody(this->pixels.front()->getBody());
			this->pixels.front()->getBody()->SetUserData(NULL);
			this->pixels.front() = NULL;
			this->pixels.erase(this->pixels.begin());
			if (firstpoint != 0)
				firstpoint--;
		}
		else
		{
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
	if (this->pixels.size() > (firstpoint + 1))
	{
		for (int i = firstpoint; i < this->pixels.size(); i++)
		{
			if (this->pixels[i] == this->pixels.back())
			{
				b2WeldJointDef rDef;
				rDef.bodyA = this->pixels.back()->getBody();
				rDef.bodyB = this->pixels[firstpoint]->getBody();
				rDef.bodyA->SetFixedRotation(true);
				rDef.bodyB->SetFixedRotation(true);
				float dx, dy;
				dx = rDef.bodyA->GetPosition().x - rDef.bodyB->GetPosition().x;
				dy = rDef.bodyA->GetPosition().y - rDef.bodyB->GetPosition().y;
				rDef.collideConnected = true;
				rDef.localAnchorA.Set(0, 0);
				rDef.localAnchorB.Set(dx / 1.2, dy / 1.2);
				this->world->CreateJoint(&rDef);
				this->world->CreateJoint(&rDef);
			}
			this->pixels[i]->getBody()->SetType(b2_dynamicBody);
		}
		firstpoint = this->pixels.size();
	}
	else if (pixels.size() > 0)
	{
		this->pixels.back()->getBody()->SetType(b2_dynamicBody);
		firstpoint++;
	}
}

void PixelControl::update(const float& dt, bool isWay, bool popfront, int pixState, int pixType)
{
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
		pixels[i]->render(target, this->sprite);
		if (this->pixels[i]->getBody()->GetPosition().x * SCALE > this->windowX ||
			this->pixels[i]->getBody()->GetPosition().x * SCALE < 0 ||
			this->pixels[i]->getBody()->GetPosition().y * SCALE > this->windowY)
		{
			this->world->DestroyBody(this->pixels[i]->getBody());
			this->pixels[i]->getBody()->SetUserData(NULL);
			this->pixels[i] = NULL;
			this->pixels.erase(this->pixels.begin() + i);
			if (firstpoint != 0 && i < firstpoint)
				firstpoint--;

		}
	}
}
