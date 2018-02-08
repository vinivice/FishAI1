#pragma once
#include"graphics.h"
#include<vector>
#include<random>
#include<ctime>
#include<iostream> //TODO DELETE

#define CHROMOSSOMES_SIZE 6
#define STANDARD_RADIUS 0.30f

class Body
{
public:
	b2Body * phisicalBody;

	Body(b2World* world, float32 px, float32 py, float32 angle, std::default_random_engine& generator);
	static bool init(GLint bodyResolution, Shader shaderInput);
	void draw(Camera* camera);
	
private:
	static GLuint innerVAO, outerVAO;
	static GLuint numberOuterVertices, numberInnerVertices;
	static Shader shader;
	static const GLfloat borderSize; //Interval (0, 1). Test Values
	static GLint resolution;
	
	//TODO Change to uchar and check where it is used for problems
	//size		type		definition
	//1			GLubyte		8 flags [x, x, x, x, x, x, x, x]
	//1			GLbyte		radius parameter [ radius = standardRadius * (1 + parameter/255)]
	//3			3*GLubyte	color RGB [ color = value / 255] . alpha = 1
	//1			GLbyte		power parameter [ power = standardPower * (1 + parameter/255) ]
	GLubyte chromossomes[CHROMOSSOMES_SIZE]; //TODO Change size of chromossomes, map chromossomes
	
	//Chromossomes translators
	//color in float [0, 1]
	GLfloat color[3];

	static void createStructures(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices);
	static void createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices);
};
