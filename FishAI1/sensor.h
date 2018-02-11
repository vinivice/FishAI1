#pragma once
#include <iostream>
#include <Box2D\Box2D.h>

class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
};