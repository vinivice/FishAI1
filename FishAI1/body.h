#pragma once
#include"graphics.h"
#include<vector>
#include<iostream> //TODO DELETE

class Body
{
public:
	void init(GLint bodyResolution);
	void draw(Camera camera);

private:
	static GLuint VAO, VBO, EBO; //TODO Check if VBO and EBO are needed
	static bool structureIsCreated;
	static Shader shader;
	static const GLfloat borderSize; //Interval (0, 1)

	//TODO Check this. Probably not needed. Use VAO instead.
	static std::vector<b2Vec2> bodyVertices;

	GLubyte chromossomes[10]; //TODO Change size of chromossomes
	
	void calculateBodyVertices(b2Vec2* vertices, GLint resolution);
	void createStructure(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices);
	//void createStructure(GLint bodyResolution, GLfloat *v, GLuint *i);
};
