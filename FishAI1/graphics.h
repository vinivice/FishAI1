#pragma once
#include<iostream>
#include<GL\glew.h>
#include<Box2D\Box2D.h>

class Camera
{
public:
	Camera();

	int32 windowWidth;
	int32 windowHeight;
	
private:
	b2Vec2 position;
	float32 extension; //Distance viewed, in world coordinates, from top to bottom of the screen
	float32 zoom;

};

extern Camera mainCamera;

//-------------

class Shader
{
public:
	Shader(char* vsCode, char* fsCode);
	bool createShaderProgram();
	GLuint shaderPTR;

private:
	char* vertexShaderCode;
	char* fragmentShaderCode;
	
	GLuint createShader(const char* shaderCode, GLenum type);
	

};