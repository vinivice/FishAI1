#pragma once
#include "graphics.h"
#include "definitions.h"

#include <vector>

class Seed
{
public:
	Seed(b2World* world, GLubyte* chromossomeInput, GLushort category, GLfloat px, GLfloat py, GLfloat vx, GLfloat vy, GLuint resolution);

	b2Body* phisicalBody;
private:
	GLubyte chromossome[CHROMOSSOMES_SIZE];
};