#include"graphics.h"

Camera mainCamera;

Camera::Camera()
{
	this->windowWidth = 800;
	this->windowHeight = 600;
	this->zoom = 1.0f;
	this->position = b2Vec2(0.0f, 0.0f);
	this->extension = 25.0f; //Test values


	this->updateProjectionMatrix();
}

void Camera::updateProjectionMatrix()
{
	GLfloat aspect = this->windowWidth / this->windowHeight;

	b2Vec2 extent(aspect*this->extension, this->extension);

	extent *= this->zoom;

	b2Vec2 lowerLeft = this->position - extent;
	b2Vec2 topRight = this->position + extent;

	this->projectionMatrix[0] = 2.0f / (topRight.x - lowerLeft.x);
	this->projectionMatrix[1] = 0.0f;
	this->projectionMatrix[2] = 0.0f;
	this->projectionMatrix[3] = 0.0f;

	this->projectionMatrix[4] = 0.0f;
	this->projectionMatrix[5] = 2.0f / (topRight.y - lowerLeft.y);
	this->projectionMatrix[6] = 0.0f;
	this->projectionMatrix[7] = 0.0f;

	this->projectionMatrix[8] = 0.0f;
	this->projectionMatrix[9] = 0.0f;
	this->projectionMatrix[10] = 1.0f;
	this->projectionMatrix[11] = 0.0f;

	this->projectionMatrix[12] = -(topRight.x + lowerLeft.x) / (topRight.x - lowerLeft.x);
	this->projectionMatrix[13] = -(topRight.y + lowerLeft.y) / (topRight.y - lowerLeft.y);
	this->projectionMatrix[14] = 0.0f;
	this->projectionMatrix[15] = 1.0f;
}

b2Vec2 Camera::convertWorldToScreen(b2Vec2 worldPosition)
{
	GLfloat aspect = this->windowWidth / this->windowHeight;
	
	b2Vec2 extent(aspect*this->extension, this->extension);
	
	extent *= this->zoom;

	b2Vec2 lowerLeft = this->position - extent;
	b2Vec2 topRight = this->position + extent;

	b2Vec2 screenPosition;

	screenPosition.x = (worldPosition.x - lowerLeft.x) / (topRight.x - lowerLeft.x);
	screenPosition.y = (worldPosition.y - lowerLeft.y) / (topRight.y - lowerLeft.y);

	screenPosition.x = screenPosition.x * this->windowWidth;
	screenPosition.y = (1.0f - screenPosition.y) * this->windowHeight;

	return screenPosition; 

}

void Camera::zoomIn()
{
	this->zoom -= 0.1f;
	if (this->zoom < 0.1f)
	{
		this->zoom = 0.1f;
	}
	this->updateProjectionMatrix();
}

void Camera::zoomOut()
{
	this->zoom += 0.1f;
	this->updateProjectionMatrix();
}

void Camera::move(GLfloat dx, GLfloat dy)
{
	this->position += this->zoom * b2Vec2(dx, dy);
	this->updateProjectionMatrix();
}

//-------------------------------------------

Shader::Shader()
{
	this->vertexShaderCode = NULL;
	this->fragmentShaderCode = NULL;
}
Shader::Shader(const char* vsCode, const char* fsCode)
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
		if (type == GL_VERTEX_SHADER)
		{
			std::cout << "VERTEX SHADER COMPILATION FAIL\n";
		}
		else if (type == GL_FRAGMENT_SHADER)
		{
			std::cout << "FRAGMENT SHADER COMPILATION FAIL\n";
		}
		return 0;
	}
	return shaderID;
}

bool Shader::createShaderProgram()
{
	GLuint vs = createShader(this->vertexShaderCode, GL_VERTEX_SHADER);
	GLuint fs = createShader(this->fragmentShaderCode, GL_FRAGMENT_SHADER);
//	getchar();

	if (vs == 0 || fs == 0)
	{
		std::cout << "SHADER PROGRAM FAIL. SHADER DIDN'T COMPILE\n";
		return false;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	
	//TODO check if need something here
	
	GLint linkOk = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkOk);
	if (linkOk == GL_FALSE)
	{
		std::cout << "SHADER PROGRAM FAIL. PROGRAM DIDN'T LINK\n";
		return false;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);


	this->shaderProgram = program;
	return true;
}