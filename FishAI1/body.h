#pragma once
#include"graphics.h"
#include<vector>
#include<iostream> //TODO DELETE

class Body
{
public:
	Body(GLint bodyResolution);

	void draw(Camera camera);
private:
	static Shader shader;
	static std::vector<b2Vec2> bodyVertices;

	GLubyte chromossomes[10]; //TODO Change size of chromossomes
	void calculateBodyVertices(b2Vec2* vertices, GLint resolution);
};
