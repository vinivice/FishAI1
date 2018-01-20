#include"graphics.h"

Camera mainCamera;

Camera::Camera()
{
	this->windowWidth = 800;
	this->windowHeight = 600;
	this->zoom = 1.0f;
	this->position = b2Vec2(0.0f, 0.0f);
	this->extension = 25.0f; //Test values
}

//---------

Shader::Shader(char* vsCode, char* fsCode)
{
	this->vertexShaderCode = vsCode;
	this->fragmentShaderCode = fsCode;
}

GLuint Shader::createShader(const char* shaderCode, GLenum type)
{
	GLuint shaderID = glCreateShader(type);

	const char* codes[] = { shaderCode }; //TODO check why it is needed
	glShaderSource(shaderID, 1, codes, NULL);
	glCompileShader(shaderID);
	
	GLint compileOk = GL_FALSE;

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileOk);
	
	if (compileOk == GL_FALSE)
	{
		std::cout << "SHADER COMPILATION FAIL\n";
		return 0;
	}
	return shaderID;
}

bool Shader::createShaderProgram()
{
	GLuint vs = createShader(this->vertexShaderCode, GL_VERTEX_SHADER);
	GLuint fs = createShader(this->fragmentShaderCode, GL_FRAGMENT_SHADER);
	getchar();

	if (vs == 0 || fs == 0)
	{
		return false;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	//TODO check if need something here

	glDeleteShader(vs);
	glDeleteShader(fs);

	GLint linkOk = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkOk);
	if (linkOk == GL_FALSE)
	{
		std::cout << "SHADER PROGRAM FAIL\n";
		return false;
	}

	this->shaderPTR = program;
	return true;
}