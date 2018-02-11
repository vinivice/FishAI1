#pragma once
#include "graphics.h"
#include "definitions.h"

class Ring
{
public:
	Ring(b2World* world, float32 radius, unsigned int resolution, Shader shaderInput);
	void draw(Camera* camera);
private:
	GLuint VAO;
	GLuint numberVertices;
	Shader shader;
	b2Body * ringBody;
	GLfloat color[3];
};