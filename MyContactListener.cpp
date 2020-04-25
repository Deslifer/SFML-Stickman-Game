#include "MyContactListener.h"
#include "PhysicComponent.h"

MyContactListener::MyContactListener(b2Fixture* mplatformFixture)
:m_platformFixture(mplatformFixture)
{
}

MyContactListener::~MyContactListener()
{
}

void MyContactListener::BeginContact(b2Contact* contact)
{
	
}

void MyContactListener::EndContact(b2Contact* contact)
{
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	b2Fixture* fixtureA = contact->GetFixtureA();

	b2Fixture* fixtureB = contact->GetFixtureB();


	//check if one of the fixtures is the platform
	b2Fixture* platformFixture = NULL;
	b2Fixture* otherFixture = NULL;

	if (fixtureA->GetUserData() == (void*)"platform") {
		platformFixture = fixtureA;
		otherFixture = fixtureB;
	}
	else if (fixtureB->GetUserData() == (void*)"platform") {
		platformFixture = fixtureB;
		otherFixture = fixtureA;
	}
	if (!platformFixture)
		return;
	b2Body* platformBody = platformFixture->GetBody();
	b2Body* otherBody = otherFixture->GetBody();

	int numPoints = contact->GetManifold()->pointCount;
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	//check if contact points are moving downward
	for (int i = 0; i < numPoints; i++) {
		b2Vec2 pointVel = otherBody->GetLinearVelocityFromWorldPoint(worldManifold.points[i]);
		if (pointVel.y > 0)
		{
			//contact->SetEnabled(true);
			return;
		}
	}

	contact->SetEnabled(false);
}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
}
