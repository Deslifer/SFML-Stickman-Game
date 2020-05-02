#include "PhysicComponent.h"

PhysicComponent::PhysicComponent(
	b2World* world,sf::Sprite& sprite
)
	:sprite(sprite),
	world(world)
{

	
	this->width = this->sprite.getTextureRect().width;
	this->height = this->sprite.getTextureRect().height;
	this->bdef.type = b2_dynamicBody;
	this->bdef.position.Set(400.f / SCALE, 400.f / SCALE);
	this->body = world->CreateBody(&bdef);
	
	//Filter
	b2Filter f;
	f.categoryBits= CATEGORY_BODY;
	f.maskBits = MASK_BODY;
	//Filter

	b2Fixture* bfix;
	this->shape.SetAsBox((9 * 0.5f - b2_polygonRadius) / SCALE, 30 / SCALE);
	bfix = this->body->CreateFixture(&shape, 1);
	bfix->SetFilterData(f);
	bfix->SetUserData((void*)"body");

	b2CircleShape circle;
	circle.m_radius = (22 - b2_polygonRadius) / SCALE;
	circle.m_p.Set(0,(55.5)/SCALE);

	this->body->SetFixedRotation(true);
	bfix = this->body->CreateFixture(&circle, 1);
	bfix->SetFilterData(f);
	bfix->SetUserData((void*)"legs");

	circle.m_radius = (31.5 - b2_polygonRadius) / SCALE;
	circle.m_p.Set(0, (-45.5) / SCALE);
	bfix = this->body->CreateFixture(&circle, 1);
	bfix->SetFilterData(f);
	bfix->SetUserData((void*)"head");

	//SHIELDS
	b2CircleShape circle3;
	circle3.m_radius = ((163.f-5.f)/2.f) / SCALE;
	bfix = this->body->CreateFixture(&circle3, -1);
	f.categoryBits = CATEGORY_BODY_SHIELD;
	f.maskBits = -1;
	bfix->SetFilterData(f);
	bfix->SetUserData((void*)"shield");
	//SHIELDS

	this->body->SetUserData((void*)"player");
}

PhysicComponent::~PhysicComponent()
{

}

sf::Vector2f PhysicComponent::getPosition()
{
	return sf::Vector2f(this->body->GetPosition().x*SCALE, this->body->GetPosition().y*SCALE);
}

b2Vec2 PhysicComponent::getVelocity()
{
	return this->body->GetLinearVelocity();
}

float PhysicComponent::getAngle()
{
	return this->body->GetAngularVelocity()*DEG;
}

b2Body* PhysicComponent::getBody()
{
	return this->body;
}

bool PhysicComponent::onGround()
{
	b2Vec2 pos = body->GetPosition();
	pos.y += (163 / 2 - 3) / SCALE;
	for (b2Body* it = world->GetBodyList(); it != 0; it = it->GetNext())
		if (it->GetUserData() == (void*)"platform" || it->GetUserData() == (void*)"box" || it->GetUserData() == (void*)"wall")
		{
			for (b2Fixture* f = it->GetFixtureList(); f != 0; f = f->GetNext())
				if (f->TestPoint(pos))
					return true;
		}
	return false;
}

