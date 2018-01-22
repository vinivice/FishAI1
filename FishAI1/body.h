#pragma once
#include"graphics.h"
#include<vector>
#include<iostream> //TODO DELETE

class Body
{
public:
	bool init(GLint bodyResolution, Shader shaderInput);
	void draw(Camera* camera);
	b2Body* phisicalBody; //TODO Constructor to add phisicalBody

private:
	static GLuint VAO;// , VBO, EBO; //TODO Check if VBO and EBO are needed
	//static bool structureIsCreated;
	static Shader shader;
	static const GLfloat borderSize; //Interval (0, 1)
	static GLint resolution;

	GLubyte chromossomes[10]; //TODO Change size of chromossomes
	
	//void calculateBodyVertices(b2Vec2* vertices, GLint resolution);
	void createStructure(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
	void createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
	//void createStructure(GLint bodyResolution, GLfloat *v, GLuint *i);
};
