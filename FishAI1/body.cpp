#include"body.h"
#include <cmath>

//bool Body::structureIsCreated = false;
const GLfloat Body::borderSize = 0.2f;
GLuint Body::VAO;
Shader Body::shader;
GLuint Body::numberVertices;

Body::Body(b2World* world)
{
	b2BodyDef bdDef;
	bdDef.type = b2_dynamicBody;
	bdDef.position.Set(0.0f, 4.0f);
	this->phisicalBody = world->CreateBody(&bdDef);
}

void Body::createStructure(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	GLfloat theta = 2 * b2_pi / bodyResolution;
	GLfloat sinValue = std::sin(theta);
	GLfloat cosValue = std::cos(theta);

	GLfloat x = 1.0f, y = 0.0f, x2, y2;
		
	//std::vector<GLfloat> vertices;
	//std::vector<GLuint> indices;
	GLfloat innerRadius = 1.0f - this->borderSize;
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
	for (k = 0; k < 2*bodyResolution; k+=2)
	{
		//Triangle 1
		indices.push_back(k%(2*bodyResolution));
		indices.push_back((k+1) % (2*bodyResolution));
		indices.push_back((k+2) % (2*bodyResolution));
		//Triangle 2  
		indices.push_back((k+2) % (2*bodyResolution));
		indices.push_back((k+1) % (2*bodyResolution));
		indices.push_back((k+3) % (2*bodyResolution));
	}

	GLfloat eyeScaleConstant = 0.9;
	vertices.push_back(0.0f); 
	vertices.push_back(0.0f);
	vertices.push_back(eyeScaleConstant * innerRadius * sqrt(3.0f) / 2.0f);
	vertices.push_back(eyeScaleConstant * innerRadius / 2.0f);
	vertices.push_back(eyeScaleConstant * innerRadius * sqrt(3.0f) / 2.0f);
	vertices.push_back(-eyeScaleConstant * innerRadius / 2.0f);

	indices.push_back(2 * bodyResolution);
	indices.push_back(2 * bodyResolution + 1);
	indices.push_back(2 * bodyResolution + 2);

	this->numberVertices = indices.size();
	/*
	//TODO DELETE
	//v = vertices.data();
	for (int j = 0; j < vertices.size(); j++)
	{
		std::cout << vertices[j] << std::endl;
	}
	//i = indices.data();
	for (int j = 0; j < indices.size(); j+=3)
	{
		std::cout << indices[j] << ", " << indices[j+1] << ", " << indices[j+2] << std::endl;
	}
	getchar();	
	*/
}

void Body::createVAO(std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	GLuint VBO, EBO;
	GLfloat *v;
	GLuint *i, vSize, iSize;

	v = vertices.data();
	vSize = vertices.size() * sizeof(GLfloat);

	i = indices.data();
	iSize = indices.size() * sizeof(GLuint);

	glGenVertexArrays(1, &(this->VAO)); //PROBLEM HERE
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, i, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//this->structureIsCreated = true;	
}


bool Body::init(GLint bodyResolution, Shader shaderInput)
{
	this->shader = shaderInput;
	this->shader.createShaderProgram();

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	this->createStructure(bodyResolution, vertices, indices);
	this->createVAO(vertices, indices);
	return true;
}

void Body::draw(Camera* camera)
{
	//0 Position
	//1 Color
	//2 Model matrix
	//3 Projection matrix
	//View matrix = identity
	b2Transform transform = this->phisicalBody->GetTransform();
	
	GLfloat model[16];

	model[0] = transform.q.c;
	model[1] = transform.q.s;
	model[2] = 0.0f;
	model[3] = 0.0f;

	model[4] = -transform.q.s;
	model[5] = transform.q.c;
	model[6] = 0.0f;
	model[7] = 0.0f;

	model[8] = 0.0f;
	model[9] = 0.0f;
	model[10] = 1.0f;
	model[11] = 0.0f;

	model[12] = transform.p.x;
	model[13] = transform.p.y;
	model[14] = 0.0f;
	model[15] = 1.0f;

	
	glUseProgram(this->shader.shaderProgram);

	GLfloat color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	
	glUniform4fv(1, 1, color);
	glUniformMatrix4fv(2, 1, GL_FALSE, model);
	glUniformMatrix4fv(3, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->numberVertices, GL_UNSIGNED_INT, 0);



	//clean up
	glUseProgram(0);
}
