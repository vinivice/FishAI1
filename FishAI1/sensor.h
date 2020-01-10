#pragma once
#include <iostream>
#if OS == linux
    #include<Box2D/Box2D.h>
#else
    #include<Box2D\Box2D.h>
#endif

#include "body.h"
#include "definitions.h"

//This class handles contact functions
class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
};
