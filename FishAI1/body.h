#pragma once
#include"graphics.h"
#include<vector>
#include<iostream> //TODO DELETE

class Body
{
public:
	Body(b2World* world);
	bool init(GLint bodyResolution, Shader shaderInput);
	void draw(Camera* camera);
	b2Body* phisicalBody; //TODO Constructor to add phisicalBody

private:
	static GLuint VAO;
	static GLuint numberVertices;
	static Shader shader;
	static const GLfloat borderSize; //Interval (0, 1). Test Values
	static GLint resolution;

	GLubyte chromossomes[10]; //TODO Change size of chromossomes, map chromossomes
	
	void createStructure(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
	void createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
};
