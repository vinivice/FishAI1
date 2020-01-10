#pragma once
#include<iostream>

#if OS == linux
    #include<GL/glew.h>
    #include<Box2D/Box2D.h>
#else
    #include<GL\glew.h>
    #include<Box2D\Box2D.h>
#endif

class Camera
{
public:
	Camera();
	b2Vec2 convertWorldToScreen(b2Vec2 worldPosition);
	void updateProjectionMatrix();
	void zoomIn();
	void zoomOut();
	void move(GLfloat dx, GLfloat dy);

	GLint windowWidth;
	GLint windowHeight;
	GLfloat projectionMatrix[16];
	
private:
	b2Vec2 position;
	GLfloat extension; //Distance viewed, in world coordinates, from top to center of the screen
	GLfloat zoom;

};

extern Camera mainCamera;

//-------------

class Shader
{
public:
	Shader();
	Shader(const char* vsCode, const char* fsCode);
	bool createShaderProgram();
	GLuint shaderProgram;

private:
	const char* vertexShaderCode;
	const char* fragmentShaderCode;
	
	GLuint createShader(const char* shaderCode, GLenum type);
	

};
