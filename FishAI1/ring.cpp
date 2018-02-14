#include "ring.h"
#include <vector>

Ring::Ring(b2World* world, float32 radius, unsigned int resolution, Shader shaderInput)
{
	this->shader = shaderInput;
	shader.createShaderProgram();

	GLfloat theta = 2 * b2_pi / resolution;
	GLfloat sinValue = std::sin(theta);
	GLfloat cosValue = std::cos(theta);

	GLfloat x = 1.0f, y = 0.0f, x2, y2;

	std::vector<GLfloat> vertices;

	int k;
	for (k = 0; k < resolution; k++)
	{
		vertices.push_back(radius*x);
		vertices.push_back(radius*y);
		
		x2 = cosValue * x - sinValue * y;
		y2 = sinValue * x + cosValue * y;
		x = x2;
		y = y2;
	}

	b2BodyDef ringDef;
	ringDef.type = b2_staticBody;

	this->ringBody = world->CreateBody(&ringDef);

	b2EdgeShape shape;

	b2FixtureDef myFixtureDef;

	myFixtureDef.shape = &shape;
	myFixtureDef.density = 0.0f;
	myFixtureDef.restitution = 1.0f;
	myFixtureDef.friction = 0.2f;
	myFixtureDef.filter.groupIndex = -1;
	myFixtureDef.filter.categoryBits = RING_CATEGORY;

	for (k = 0; k < resolution; k++)
	{
		shape.Set(b2Vec2(vertices[(2 * k) % (2 *resolution)], vertices[(2 * k + 1) % (2 * resolution)]), b2Vec2(vertices[(2 * k + 2) % (2 * resolution)], vertices[(2 * k + 3) % (2 * resolution)]));
		ringBody->CreateFixture(&myFixtureDef);
	}

	this->numberVertices = vertices.size() / 2;

	GLuint VBO;
	GLfloat *v;
	GLuint vSize;

	v = vertices.data();
	vSize = vertices.size() * sizeof(GLfloat);

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->color[0] = 1.0f; //R
	this->color[1] = 1.0f; //G
	this->color[2] = 1.0f; //B
}

void Ring::draw(Camera* camera)
{
	//0 position
	//1 Color
	//2 Projection matrix
	//View and model matrix = identity
	glUseProgram(this->shader.shaderProgram);

	glUniform3fv(1, 1, this->color);
	glUniformMatrix4fv(2, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINE_LOOP, 0, this->numberVertices);
	//glDrawElements(GL_TRIANGLE_FAN, this->numberInnerVertices, GL_UNSIGNED_INT, 0);

	//clean up
	glUseProgram(0);
}
