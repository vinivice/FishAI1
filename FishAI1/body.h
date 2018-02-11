#pragma once
#include "graphics.h"
#include <vector>
#include <random>
#include <ctime>
#include <iostream> //TODO DELETE

#define CHROMOSSOMES_SIZE 7
#define STANDARD_RADIUS 0.30f
#define STANDARD_POWER 1.0f
#define LINEAR_DAMPING 0.25f
#define ANGULAR_DAMPING 0.05f

class Body 
{
public:
	b2Body * phisicalBody;

	Body(b2World* world, float32 px, float32 py, float32 angle, std::default_random_engine& generator);
	static bool init(GLint bodyResolution, Shader shaderInput);
	void drawBody(Camera* camera);
	void drawSensors(Camera* camera);
	void useLeftPropulsor(bool reverse);
	void useRightPropulsor(bool reverse);
	//void BeginContact(b2Contact* contact);
	
private:
	static GLuint innerVAO, outerVAO, earVAO;
	static GLuint numberOuterVertices, numberInnerVertices, numberEarVertices;
	static Shader shader;
	static const GLfloat borderSize; //Interval (0, 1). Test Values
	static GLint resolution;
	
	//size		offset		type		definition
	//1			0			GLubyte		8 flags [x, x, x, x, x, x, x, x]
	//1			1			GLbyte		bodyRadius parameter [ radius = standardRadius * (1 + parameter/255)]
	//3			2			3*GLubyte	color RGB [ color = value / 255] . alpha = 1
	//1			5			GLbyte		power parameter [ power = standardPower * (1 + parameter/255) ]
	//1			6			GLbyte		sensorRadius parameter [ power = standardPower * (1 + parameter/255) ]
	GLubyte chromossomes[CHROMOSSOMES_SIZE]; //TODO Change size of chromossomes, map chromossomes
	
	//Chromossomes translators
	//color in float [0, 1]
	GLfloat color[3];
	GLfloat power, bodyRadius, earRadius;

	static void createStructures(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices, std::vector<GLuint> &earIndices);
	static void createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices, std::vector<GLuint> &earIndices);
};
