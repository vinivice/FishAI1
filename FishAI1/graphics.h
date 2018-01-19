#pragma once
#include<iostream>
#include<GL\glew.h>
#include<Box2D\Box2D.h>

class Camera
{
public:
	Camera()
	{
		this->windowWidth = 800;
		this->windowHeight = 600;
		this->zoom = 1.0f;
		this->position = b2Vec2(0.0f, 0.0f);
		this->extension = 25.0f; //Test values
	}

	int32 windowWidth;
	int32 windowHeight;
	
private:
	b2Vec2 position;
	float32 extension; //Distance viewed, in world coordinates, from top to bottom of the screen
	float32 zoom;

};

extern Camera mainCamera;

class Shader
{
public:
	Shader(char* vsCode, char* fsCode)
	{
		this->vertexShaderCode = vsCode;
		this->fragmentShaderCode = fsCode;
	}
	bool createShaderProgram();
	GLuint shaderPTR;

private:
	char* vertexShaderCode;
	char* fragmentShaderCode;
	
	GLuint createShader(const char* shaderCode, GLenum type);
	

};