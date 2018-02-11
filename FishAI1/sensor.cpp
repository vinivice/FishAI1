#include "sensor.h"

//Sensor functions
void MyContactListener::BeginContact(b2Contact* contact)
{
	b2Fixture* FA = contact->GetFixtureA();
	b2Fixture* FB = contact->GetFixtureB();
	if (FA->GetFilterData().categoryBits == RING_CATEGORY || FB->GetFilterData().categoryBits == RING_CATEGORY)
	{
		//std::cout << "HIT RING\n";
		return;
	}
	else if (FA->GetFilterData().categoryBits == FISH_NEAR_SENSOR_CATEGORY || FB->GetFilterData().categoryBits == FISH_NEAR_SENSOR_CATEGORY)
	{
		Body* fishBody;
		if (FA->GetFilterData().categoryBits == FISH_NEAR_SENSOR_CATEGORY)
		{
			fishBody = (Body*)FA->GetBody()->GetUserData();
		}
		else
		{
			fishBody = (Body*)FB->GetBody()->GetUserData();
		}
		fishBody->increaseNumberNearThings();
		//std::cout << fishBody->numberFishesHeard << std::endl;

		std::cout << "SENSOR IN\n";
	}
	else
	{
		std::cout << "HELLO\n";
	}
}

void MyContactListener::EndContact(b2Contact* contact)
{
	b2Fixture* FA = contact->GetFixtureA();
	b2Fixture* FB = contact->GetFixtureB();
	if (FA->GetFilterData().categoryBits == RING_CATEGORY || FB->GetFilterData().categoryBits == RING_CATEGORY)
	{
		//std::cout << "HIT RING\n";
		return;
	}
	else if (FA->GetFilterData().categoryBits == FISH_NEAR_SENSOR_CATEGORY || FB->GetFilterData().categoryBits == FISH_NEAR_SENSOR_CATEGORY)
	{
		Body* fishBody;
		if (FA->GetFilterData().categoryBits == FISH_NEAR_SENSOR_CATEGORY)
		{
			fishBody = (Body*)FA->GetBody()->GetUserData();
		}
		else
		{
			fishBody = (Body*)FB->GetBody()->GetUserData();
		}
		fishBody->decreaseNumberNearThings();
		//std::cout << fishBody->numberFishesHeard << std::endl;

		std::cout << "SENSOR OUT\n";
	}
	else
	{
		std::cout << "HELLO\n";
	}
}
