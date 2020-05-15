#pragma once
#include "box2d/box2d.h"
#include <iostream>
#include "PhysicComponent.h"

class MyContactListener :
	public b2ContactListener
{
private:
	b2Fixture* m_platformFixture;
public:
	MyContactListener(b2Fixture* mplatformFixture);
	virtual ~MyContactListener();
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

