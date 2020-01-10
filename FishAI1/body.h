#pragma once
#include "graphics.h"
#include "definitions.h"

#include <vector>
#include <random>
#include <ctime>
#include <iostream> //TODO DELETE


class Body 
{
public:
	b2Body * phisicalBody;

	Body(b2World* world, float32 px, float32 py, float32 angle);
	static bool init(GLint bodyResolution, Shader shaderInput, Shader eyeShaderInput, std::default_random_engine* gen);
	void drawBody(Camera* camera);
	void drawSensors(Camera* camera);
	void drawEyes(Camera* camera);
	void useLeftPropulsor(bool reverse);
	void useRightPropulsor(bool reverse);
	void increaseNumberNearThings();
	void decreaseNumberNearThings();
	void update(b2World *world);


	
protected:
	static GLuint innerVAO, outerVAO, nearSensorVAO, eyeVAO;
	static GLuint numberOuterVertices, numberInnerVertices, numberNearSensorVertices;
	static Shader shader;
	static Shader eyeShader;
	static const GLfloat borderSize; //Interval (0, 1). Test Values
	static GLint resolution;
	static std::default_random_engine *generator;
	
	//size		offset		type		definition
	//1			0			GLubyte		8 flags [x, x, x, x, x, x, x, x]
	//1			1			GLbyte		bodyRadius parameter [ radius = standardRadius * (1 + parameter/255)]
	//3			2			3*GLubyte	color RGB [ color = value / 255] . alpha = 1
	//1			5			GLbyte		power parameter [ power = standardPower * (1 + parameter/255) ]
	//1			6			GLbyte		sensorRadius parameter [ radius = 2 * standardRadius * (1 + parameter/255) ]
	//1			7			GLbyte		eyeLenght parameter [ power = 4 * standardRadius * (1 + parameter/255) ]
	GLubyte chromossomes[CHROMOSSOMES_SIZE]; //TODO Change size of chromossomes, map chromossomes
	
	//Chromossomes translators
	//color in float [0, 1]
	GLfloat color[3];
	GLfloat power, bodyRadius, nearSensorRadius, eyeLenght;

	//Sensors readings
	GLuint numberNearThings;
	GLfloat eyeFraction[5];

    //Eyes directions
    GLfloat sin[5];
    GLfloat cos[5];

	static void createStructures(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices, std::vector<GLuint> &nearSensorIndices, std::vector<GLuint> &eyeIndices);
	static void createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices, std::vector<GLuint> &nearSensorIndices, std::vector<GLuint> &eyeIndices);
};
