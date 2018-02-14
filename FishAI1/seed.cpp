#include "seed.h"

Seed::Seed(b2World* world, GLubyte* chromossomeInput, GLushort category, GLfloat px, GLfloat py, GLfloat vx, GLfloat vy, GLuint resolution)
{
	int i;
	for (i = 0; i < CHROMOSSOMES_SIZE; i++)
	{
		this->chromossome[i] = chromossomeInput[i];
	}

	b2BodyDef seedDef;
	seedDef.type = b2_dynamicBody;
	seedDef.position.Set(px, py);
	seedDef.linearVelocity.Set(vx, vy);
	seedDef.userData = this;
	seedDef.linearDamping = LINEAR_DAMPING;
	seedDef.angularDamping = ANGULAR_DAMPING;
	this->phisicalBody = world->CreateBody(&seedDef);

	b2CircleShape seedShape;
	seedShape.m_p.SetZero();

	b2FixtureDef seedFixtureDef;
	seedFixtureDef.shape = &seedShape;

	seedShape.m_radius = SEED_RADIUS;
	seedFixtureDef.density = 0.5f;



	if (category == SEX_SEED_CATEGORY)
	{
		seedFixtureDef.filter.categoryBits = SEX_SEED_CATEGORY;
		seedFixtureDef.filter.maskBits = FISH_BODY_CATEGORY | RING_CATEGORY;
	}
	else
	{
		seedFixtureDef.filter.categoryBits = ASEX_SEED_CATEGORY;
		seedFixtureDef.filter.maskBits = RING_CATEGORY;

	}
	std::vector<GLfloat> vertices;

	GLfloat theta = 2 * b2_pi / resolution;
	GLfloat sinValue = std::sin(theta);
	GLfloat cosValue = std::cos(theta);

	GLfloat x = 1.0f, y = 0.0f, x2, y2;

	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	
	//Vertices and indices 
	int k;
	for (k = 0; k < resolution; k++)
	{
		vertices.push_back(x);
		vertices.push_back(y);
		
		x2 = cosValue * x - sinValue * y;
		y2 = sinValue * x + cosValue * y;
		x = x2;
		y = y2;
	}

}
