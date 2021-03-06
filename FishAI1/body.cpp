#include "body.h"
#include <cmath>

//const GLfloat Body::borderSize = 0.2f;
GLuint Body::innerVAO;
GLuint Body::outerVAO;
GLuint Body::nearSensorVAO;
GLuint Body::eyeVAO;
Shader Body::shader;
Shader Body::eyeShader;
GLuint Body::numberInnerVertices;
GLuint Body::numberOuterVertices;
GLuint Body::numberNearSensorVertices;
GLint Body::resolution;
std::default_random_engine* Body::generator;


Body::Body(b2World* world, float32 px, float32 py, float32 angle)
{
	//Random body constructor
	std::uniform_int_distribution<unsigned short> distribution(0, 255);
	for (int i = 0; i < CHROMOSSOMES_SIZE; i++)
	{
		this->chromossomes[i] = (unsigned char)distribution(*(this->generator));
		//std::cout << std::hex << +this->chromossomes[i] << std::endl;
	}
    sharedConstructor(world, px, py, angle);
}

Body::Body(b2World* world, float32 px, float32 py, float32 angle, unsigned char *chromossomes)
{
	//Preexisting chromossomes constructor
	for (int i = 0; i < CHROMOSSOMES_SIZE; i++)
	{
		this->chromossomes[i] = chromossomes[i];
		//std::cout << std::hex << +this->chromossomes[i] << std::endl;
	}
    sharedConstructor(world, px, py, angle);
}
void Body::sharedConstructor(b2World* world, float32 px, float32 py, float32 angle)
{
	this->numberNearThings = 0;
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

	GLbyte brParameter = *(GLbyte*)(this->chromossomes + 1);
	GLbyte erParameter = *(GLbyte*)(this->chromossomes + 6);
	GLbyte eyeParameter = *(GLbyte*)(this->chromossomes + 7);
	this->bodyRadius = STANDARD_RADIUS * (1 + brParameter / 255.0f);
	this->nearSensorRadius = 2 * STANDARD_RADIUS * (1 + erParameter / 255.0f);
	this->eyeLenght = this->nearSensorRadius + 2 * STANDARD_RADIUS * (1 + eyeParameter / 255.0f);

	//Add near sensor fixure
	bodyShape.m_radius = this->nearSensorRadius; 
	bodyFixtureDef.density = 0.0f;
	bodyFixtureDef.isSensor= true;
	bodyFixtureDef.filter.groupIndex = -1;
	bodyFixtureDef.filter.categoryBits = FISH_NEAR_SENSOR_CATEGORY;
	this->phisicalBody->CreateFixture(&bodyFixtureDef);

	//Add body shape fixture
	bodyShape.m_radius = this->bodyRadius;
	bodyFixtureDef.density = 1.0f;
	bodyFixtureDef.isSensor = false;
	bodyFixtureDef.filter.groupIndex = 0;
	bodyFixtureDef.filter.categoryBits = FISH_BODY_CATEGORY;
	this->phisicalBody->CreateFixture(&bodyFixtureDef);

	//TODO Complete definition of body (add fixture etc). Done? Can't remember
}

void Body::createStructures(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices, std::vector<GLuint> &nearSensorIndices, std::vector<GLuint> &eyeIndices)
{
	GLfloat theta = 2 * b2_pi / bodyResolution;
	GLfloat sinValue = std::sin(theta);
	GLfloat cosValue = std::cos(theta);

	GLfloat x = 1.0f, y = 0.0f, x2, y2;
		
	GLfloat innerRadius = 1.0f - BORDER_SIZE;
	
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

	//TODO check if necessary
	nearSensorIndices.push_back(2 * bodyResolution + 2);
	innerIndices.push_back(2 * bodyResolution + 2);
	
	for (k = 0; k < 2*bodyResolution; k+=2)
	{
		//Near Sensor triangle
		nearSensorIndices.push_back(k);
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

	nearSensorIndices.push_back(0);
	innerIndices.push_back(1);

    //Triangle representing the eye, not the eye sensor
	GLfloat eyeScaleConstant = 0.9; //TODO transform definition
	vertices.push_back(eyeScaleConstant * innerRadius * sqrt(3.0f) / 2.0f); //TODO precalc constant
	vertices.push_back(eyeScaleConstant * innerRadius / 2.0f);
	vertices.push_back(eyeScaleConstant * innerRadius * sqrt(3.0f) / 2.0f);
	vertices.push_back(-eyeScaleConstant * innerRadius / 2.0f);
	vertices.push_back(0.0f); // 2*bodyRes + 2
	vertices.push_back(0.0f);

	outerIndices.push_back(2 * bodyResolution);
	outerIndices.push_back(2 * bodyResolution + 1);
	outerIndices.push_back(2 * bodyResolution + 2);

	numberInnerVertices = innerIndices.size();
	numberOuterVertices = outerIndices.size();
	numberNearSensorVertices = nearSensorIndices.size();
	
    //Eye sensor. Line from (0,0) to (1,0)
	eyeIndices.push_back(2 * bodyResolution + 2);
	eyeIndices.push_back(0);

	/*
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
	*/
}

void Body::createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &innerIndices, std::vector<GLuint> &outerIndices, std::vector<GLuint> &nearSensorIndices, std::vector<GLuint> &eyeIndices)
{
	GLuint VBO, innerEBO, outerEBO, nearSensorEBO, eyeEBO;
	GLfloat *v;
	GLuint *ii, *oi, *ei, *eyei, vSize, iiSize, oiSize, eiSize;

	v = vertices.data();
	vSize = vertices.size() * sizeof(GLfloat);

	ii = innerIndices.data();
	iiSize = innerIndices.size() * sizeof(GLuint);

	oi = outerIndices.data();
	oiSize = outerIndices.size() * sizeof(GLuint);

	ei = nearSensorIndices.data();
	eiSize = nearSensorIndices.size() * sizeof(GLuint);

	eyei = eyeIndices.data();


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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outerEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, oiSize, oi, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//nearSensorVAO
	glGenVertexArrays(1, &nearSensorVAO);

	glGenBuffers(1, &nearSensorEBO);

	glBindVertexArray(nearSensorVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nearSensorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eiSize, ei, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//eyeVAO
	glGenVertexArrays(1, &eyeVAO);

	glGenBuffers(1, &eyeEBO);

	glBindVertexArray(eyeVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eyeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), eyei, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	//Clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


bool Body::init(GLint bodyResolution, Shader shaderInput, Shader eyeShaderInput, std::default_random_engine* gen)
{
	generator = gen;
	shader = shaderInput;
	shader.createShaderProgram();

	eyeShader = eyeShaderInput;
	eyeShader.createShaderProgram();


	resolution = bodyResolution;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> innerIndices;
	std::vector<GLuint> outerIndices;
	std::vector<GLuint> nearSensorIndices;
	std::vector<GLuint> eyeIndices;

	createStructures(bodyResolution, vertices, innerIndices, outerIndices, nearSensorIndices, eyeIndices);
	createVAO(vertices, innerIndices, outerIndices, nearSensorIndices, eyeIndices);
	return true;
}

void Body::drawBody(Camera* camera)
{
	b2Transform transform = this->phisicalBody->GetTransform();

	GLfloat model[16];
	GLfloat r = this->bodyRadius;

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
	GLfloat r = this->nearSensorRadius;

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

	//Draw near sensor
	glUseProgram(this->shader.shaderProgram);
	GLfloat sensorColor[3] = { 0.0f, 1.0f, 0.0f };

	glUniform3fv(1, 1, sensorColor);
	glUniformMatrix4fv(2, 1, GL_FALSE, model);
	glUniformMatrix4fv(3, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->nearSensorVAO);
	glDrawElements(GL_TRIANGLE_FAN, this->numberNearSensorVertices, GL_UNSIGNED_INT, 0);

	//clean up
	glUseProgram(0);
}

void Body::drawEyes(Camera* camera)
{
	b2Transform transform = this->phisicalBody->GetTransform();

	GLfloat model[16];
	GLfloat r = this->eyeLenght;

	model[2] = 0.0f;
	model[3] = 0.0f;

	model[6] = 0.0f;
	model[7] = 0.0f;

	model[8] = 0.0f;
	model[9] = 0.0f;
	model[11] = 0.0f;

	model[12] = transform.p.x;
	model[13] = transform.p.y;
	model[14] = 0.0f;
	model[15] = 1.0f;

	

	//Draw eye
	glUseProgram(this->shader.shaderProgram);
	GLfloat sensorColor[3] = { 1.0f, 1.0f, 1.0f };

	glUniform3fv(1, 1, sensorColor);
	glUniformMatrix4fv(3, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->eyeVAO); 
	
	for (int iii = 0; iii < 5; iii++)
	{
		model[0] = model[5] = r * this->cos[iii] * this->eyeFraction[iii];
		model[1] = r * this->sin[iii] * this->eyeFraction[iii];
		model[4] = -r * this->sin[iii] * this->eyeFraction[iii];
		model[10] = r * this->eyeFraction[iii];
		glUniformMatrix4fv(2, 1, GL_FALSE, model);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
	}
	
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

void Body::increaseNumberNearThings()
{
	this->numberNearThings++;
}

void Body::decreaseNumberNearThings()
{
	this->numberNearThings--;
	if (this->numberNearThings < 0)
	{
		this->numberNearThings = 0;
	}
}

void Body::update(b2World *world)
{
	b2Transform transform = this->phisicalBody->GetTransform();
    //Raytrace the eyes
    this->sin[0] = SUM_SIN(transform.q.s, transform.q.c, -SIN_30, COS_30);
    this->sin[1] = SUM_SIN(transform.q.s, transform.q.c, -SIN_15, COS_15);
    this->sin[2] = transform.q.s;
    this->sin[3] = SUM_SIN(transform.q.s, transform.q.c, SIN_15, COS_15);
    this->sin[4] = SUM_SIN(transform.q.s, transform.q.c, SIN_30, COS_30);

	this->cos[0] = SUM_COS(transform.q.s, transform.q.c, -SIN_30, COS_30);
	this->cos[1] = SUM_COS(transform.q.s, transform.q.c, -SIN_15, COS_15);
	this->cos[2] = transform.q.c;
	this->cos[3] = SUM_COS(transform.q.s, transform.q.c, SIN_15, COS_15);
	this->cos[4] = SUM_COS(transform.q.s, transform.q.c, SIN_30, COS_30);

    
    b2RayCastInput input;
    input.p1 = transform.p;
    input.maxFraction = 1;

    b2RayCastOutput output;


    for(int iii = 0; iii < 5; iii++)
    {
        input.p2 = transform.p + this->eyeLenght * b2Vec2(this->cos[iii], this->sin[iii]);
        //std::cout << input.p1.x << " , " << input.p1.y << ")(" << input.p2.x << " , " << input.p2.y << ")\n";
        float closestFraction = 1;
        for(b2Body *b=world->GetBodyList(); b; b = b->GetNext())
        {
            if(b->GetUserData() != this)
            {
                for(b2Fixture *f=b->GetFixtureList(); f; f = f->GetNext())
                {
                    if(f->IsSensor())
                    {
                        continue;
                    }
                    if(!(f->RayCast(&output, input, 0)))
                    {
                        continue;
                    }
                    if(output.fraction < closestFraction)
                    {
                        closestFraction = output.fraction;
                        //TODO add closes color here
                    }
                }
            }
        }
        this->eyeFraction[iii] = closestFraction;
    }




	/*this->eyeFraction[0] = 0.2f;
	this->eyeFraction[1] = 0.4f;
	this->eyeFraction[2] = 0.6f;
	this->eyeFraction[3] = 0.8f;
	this->eyeFraction[4] = 1.0f;
	*/
    //return;
}
