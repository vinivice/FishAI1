#pragma once
#include "graphics.h"
#include "definitions.h"

#include <vector>
#include <random>

class Seed
{
public:
	Seed(b2World* world, GLubyte* chromossomeInput, GLushort category, GLfloat px, GLfloat py, GLfloat vx, GLfloat vy);
	static bool init(GLuint resolution, Shader shaderInput, std::default_random_engine *generatorInput);
	void draw(Camera *camera);

	b2Body* phisicalBody;
private:
	GLubyte chromossome[CHROMOSSOMES_SIZE];

	GLfloat color[3];

	static Shader shader;
	static GLuint VAO;
	static GLuint numberVertices;
	static std::default_random_engine *generator;

};