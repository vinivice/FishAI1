#pragma once
#include "graphics.h"
#include "definitions.h"
#include "body.h"

#include <vector>
#include <random>

class Seed
{
public:
	Seed(b2World* world, GLubyte* chromossomeInput, GLushort category, GLfloat px, GLfloat py, GLfloat vx, GLfloat vy, GLfloat ttl);
    ~Seed();
	static bool init(GLuint resolution, Shader shaderInput, std::default_random_engine *generatorInput);
	void draw(Camera *camera);
    bool update(GLfloat timeInterval, std::vector<Body *> *fishes);

	b2Body* phisicalBody;
private:
	GLubyte chromossome[CHROMOSSOMES_SIZE];

	GLfloat color[3];
    GLfloat ttl;

    GLushort category;

	static Shader shader;
	static GLuint VAO;
	static GLuint numberVertices;
	static std::default_random_engine *generator;

};
