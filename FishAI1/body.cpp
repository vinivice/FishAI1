#include "body.h"
#include <cmath>

const GLfloat Body::borderSize = 0.2f;
GLuint Body::innerVAO;
GLuint Body::outerVAO;
GLuint Body::earVAO;
Shader Body::shader;
GLuint Body::numberInnerVertices;
GLuint Body::numberOuterVertices;
GLint Body::resolution;

Body::Body(b2World* world, float32 px, float32 py, float32 angle, std::default_random_engine& generator)
{
	//Random body constructor
	std::uniform_int_distribution<unsigned short> distribution(0, 255);
	for (int i = 0; i < CHROMOSSOMES_SIZE; i++)
	{
		this->chromossomes[i] = (unsigned char)distribution(generator);
		//std::cout << std::hex << +this->chromossomes[i] << std::endl;
	}

	//Populate translators for easier access
	this->color[0] = this->chromossomes[2] / 255.0f;
	this->color[1] = this->chromossomes[3] / 255.0f;
	this->color[2] = this->chromossomes[4] / 255.0f;
	GLbyte pParameter = *(GLbyte*)(this->chromossomes + 5);
	this->power = STANDARD_POWER * (1 + pParameter / 255.0f);


	//std::cout << std::hex << ULONG_MAX << std::endl;
	//getchar();

	//Create body
	b2BodyDef bdDef;
	bdDef.type = b2_dynamicBody;
	bdDef.position.Set(px, py);
	bdDef.angle = angle;
	bdDef.userData = this;
	bdDef.linearDamping = LINEAR_DAMPING;
	bdDef.angularDamping = ANGULAR_DAMPING;
	this->phisicalBody = world->CreateBody(&bdDef);
	
	b2CircleShape bodyShape;
	bodyShape.m_p.SetZero();
	
	b2FixtureDef bodyFixtureDef;
	bodyFixtureDef.shape = &bodyShape;

	//Add body shape fixture
	GLbyte rParameter = *(GLbyte*)(this->chromossomes + 1);
	bodyShape.m_radius = STANDARD_RADIUS * (1 + rParameter / 255.0f);
	bodyFixtureDef.density = 1.0f;
	this->phisicalBody->CreateFixture(&bodyFixtureDef);

	//Add ear sensor fixure
	bodyShape.m_radius = 2 * bodyShape.m_radius;
	bodyFixtureDef.density = 0.0f;
	this->phisicalBody->CreateFixture(&bodyFixtureDef);

	//TODO Complete definition of body (add fixture etc)
}

void Body::createStructures(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices)
{
	GLfloat theta = 2 * b2_pi / bodyResolution;
	GLfloat sinValue = std::sin(theta);
	GLfloat cosValue = std::cos(theta);

	GLfloat x = 1.0f, y = 0.0f, x2, y2;
		
	GLfloat innerRadius = 1.0f - borderSize;
	
	//Body vertices and indices 
	int k;
	for (k = 0; k < bodyResolution; k++)
	{
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(innerRadius*x);
		vertices.push_back(innerRadius*y);

		x2 = cosValue * x - sinValue * y;
		y2 = sinValue * x + cosValue * y;
		x = x2;
		y = y2;
	}

	innerIndices.push_back(2 * bodyResolution + 2);

	for (k = 0; k < 2*bodyResolution; k+=2)
	{
		//Inner triangle
		innerIndices.push_back(k + 1);
		//Outer triangle 1
		outerIndices.push_back(k%(2*bodyResolution));
		outerIndices.push_back((k+1) % (2*bodyResolution));
		outerIndices.push_back((k+2) % (2*bodyResolution));
		//Outer triangle 2  
		outerIndices.push_back((k+2) % (2*bodyResolution));
		outerIndices.push_back((k+1) % (2*bodyResolution));
		outerIndices.push_back((k+3) % (2*bodyResolution));
	}

	innerIndices.push_back(1);

	GLfloat eyeScaleConstant = 0.9;
	vertices.push_back(eyeScaleConstant * innerRadius * sqrt(3.0f) / 2.0f);
	vertices.push_back(eyeScaleConstant * innerRadius / 2.0f);
	vertices.push_back(eyeScaleConstant * innerRadius * sqrt(3.0f) / 2.0f);
	vertices.push_back(-eyeScaleConstant * innerRadius / 2.0f);
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	outerIndices.push_back(2 * bodyResolution);
	outerIndices.push_back(2 * bodyResolution + 1);
	outerIndices.push_back(2 * bodyResolution + 2);

	numberInnerVertices = innerIndices.size();
	numberOuterVertices = outerIndices.size();

	//Ear sensor vectices 
	x = 1.0f;
	y = 0.0f;
	for (k = 0; k < bodyResolution; k++)
	{
		vertices.push_back(2 * x);
		vertices.push_back(2 * y);
	
		x2 = cosValue * x - sinValue * y;
		y2 = sinValue * x + cosValue * y;
		x = x2;
		y = y2;
	}

	vertices.push_back(2.0f);
	vertices.push_back(0.0f);
}

void Body::createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices)
{
	GLuint VBO, innerEBO, outerEBO;
	GLfloat *v;
	GLuint *ii, *oi, vSize, iiSize, oiSize;

	v = vertices.data();
	vSize = vertices.size() * sizeof(GLfloat);

	ii = innerIndices.data();
	iiSize = innerIndices.size() * sizeof(GLuint);

	oi = outerIndices.data();
	oiSize = outerIndices.size() * sizeof(GLuint);

	//innerVAO
	glGenVertexArrays(1, &innerVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &innerEBO);

	glBindVertexArray(innerVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, innerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iiSize, ii, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//outerVAO
	glGenVertexArrays(1, &outerVAO);
	//glGenBuffers(1, &VBO);
	glGenBuffers(1, &outerEBO);

	glBindVertexArray(outerVAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, oiSize, oi, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//earVAO
	glGenVertexArrays(1, &earVAO);

	glBindVertexArray(earVAO);

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(2 * resolution + 2));
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


bool Body::init(GLint bodyResolution, Shader shaderInput)
{
	shader = shaderInput;
	shader.createShaderProgram();

	resolution = bodyResolution;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> innerIndices;
	std::vector<GLuint> outerIndices;

	createStructures(bodyResolution, vertices, innerIndices, outerIndices);
	createVAO(vertices, innerIndices, outerIndices);
	return true;
}

void Body::drawBody(Camera* camera)
{
	b2Transform transform = this->phisicalBody->GetTransform();

	GLfloat model[16];
	GLfloat r = this->phisicalBody->GetFixtureList()[0].GetShape()->m_radius;

	model[0] = r * transform.q.c;
	model[1] = r * transform.q.s;
	model[2] = 0.0f;
	model[3] = 0.0f;

	model[4] = -r * transform.q.s;
	model[5] = r * transform.q.c;
	model[6] = 0.0f;
	model[7] = 0.0f;

	model[8] = 0.0f;
	model[9] = 0.0f;
	model[10] = r;
	model[11] = 0.0f;

	model[12] = transform.p.x;
	model[13] = transform.p.y;
	model[14] = 0.0f;
	model[15] = 1.0f;

	GLfloat invColor[3] = { 1.0f - this->color[0], 1.0f - this->color[1], 1.0f - this->color[2] };

	//Draw body parts

	//0 Position
	//1 Color
	//2 Model matrix
	//3 Projection matrix
	//View matrix = identity
	glUseProgram(this->shader.shaderProgram);

	//Draw inner body
	glUniform3fv(1, 1, this->color);
	glUniformMatrix4fv(2, 1, GL_FALSE, model);
	glUniformMatrix4fv(3, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->innerVAO);
	glDrawElements(GL_TRIANGLE_FAN, this->numberInnerVertices, GL_UNSIGNED_INT, 0);

	//Draw outer body
	glUniform3fv(1, 1, invColor);

	glBindVertexArray(this->outerVAO);
	glDrawElements(GL_TRIANGLES, this->numberOuterVertices, GL_UNSIGNED_INT, 0);

	//clean up
	glUseProgram(0);
}

void Body::drawSensors(Camera* camera)
{
	b2Transform transform = this->phisicalBody->GetTransform();

	GLfloat model[16];
	GLfloat r = this->phisicalBody->GetFixtureList()[0].GetShape()->m_radius;

	model[0] = r * transform.q.c;
	model[1] = r * transform.q.s;
	model[2] = 0.0f;
	model[3] = 0.0f;

	model[4] = -r * transform.q.s;
	model[5] = r * transform.q.c;
	model[6] = 0.0f;
	model[7] = 0.0f;

	model[8] = 0.0f;
	model[9] = 0.0f;
	model[10] = r;
	model[11] = 0.0f;

	model[12] = transform.p.x;
	model[13] = transform.p.y;
	model[14] = 0.0f;
	model[15] = 1.0f;

	//Draw ear sensor
	glUseProgram(this->shader.shaderProgram);
	GLfloat sensorColor[3] = { 0.0f, 1.0f, 0.0f }; //TODO DELETE. magic color in shader

	glUniform3fv(1, 1, sensorColor);
	glUniformMatrix4fv(2, 1, GL_FALSE, model);
	glUniformMatrix4fv(3, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->earVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 2 * resolution + 2, resolution + 2);

	//clean up
	glUseProgram(0);
}

void Body::useLeftPropulsor(bool reverse)
{
	GLfloat r = this->phisicalBody->GetFixtureList()[0].GetShape()->m_radius;
	GLfloat theta = this->phisicalBody->GetAngle();
	b2Vec2 p = this->phisicalBody->GetPosition();
	b2Vec2 f(this->power * cosf(theta), this->power * sinf(theta));
	b2Vec2 fPoint(r*cosf(theta + b2_pi / 2), r*sinf(theta + b2_pi / 2));
	if(reverse)
	{ 
		this->phisicalBody->ApplyForce(-f, p + fPoint, true);
	}
	else
	{
		this->phisicalBody->ApplyForce(f, p + fPoint, true);
	}
}

void Body::useRightPropulsor(bool reverse)
{
	GLfloat r = this->phisicalBody->GetFixtureList()[0].GetShape()->m_radius;
	GLfloat theta = this->phisicalBody->GetAngle();
	b2Vec2 p = this->phisicalBody->GetPosition();
	b2Vec2 f(this->power * cosf(theta), this->power * sinf(theta));
	b2Vec2 fPoint(r*cosf(theta - b2_pi / 2), r*sinf(theta - b2_pi / 2));
	if (reverse)
	{
		this->phisicalBody->ApplyForce(-f, p + fPoint, true);
	}
	else
	{
		this->phisicalBody->ApplyForce(f, p + fPoint, true);
	}
}

//Sensor functions
void BeginContact(b2Contact* contact)
{
	std::cout << "HELLO\n";
}
