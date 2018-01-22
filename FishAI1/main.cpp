#include<iostream>
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include<Box2D\Box2D.h>
#include<vector>
#include<ctime>

#include"graphics.h"
#include"body.h"
#include"shader_codes.h"


/******* CALLBACK FUNCTIONS *******/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_I:
		if(action == GLFW_PRESS)
		{
			mainCamera.zoomIn();
		}
		break;
	case GLFW_KEY_O:
		if (action == GLFW_PRESS)
		{
			mainCamera.zoomOut();
		}
		break;
		//TODO add WASD to move cam 
	}
}
/**********************************/

int main(int argc, char* argv[])
{
	GLuint fps = 60;
	//TODO DELETE
	std::cout << "Hello World!\n";

	/******* HANDLE CLI OPTIONS *******/
	//TODO
	/**********************************/

	/******* GLFW AND GLEW INIT *******/
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	

	GLFWwindow *window = glfwCreateWindow(mainCamera.windowWidth, mainCamera.windowHeight, "Test 2D", nullptr, nullptr);

	glfwGetFramebufferSize(window, &mainCamera.windowWidth, &mainCamera.windowHeight);
	
	if (nullptr == window)
	{
		std::cout << "WINDOW FAIL\n";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	//-----------
	
	glewExperimental = GL_TRUE; //TODO Check what it does
	
	if(GLEW_OK != glewInit())
	{
		std::cout << "GLEW FAIL\n";
		return EXIT_FAILURE;
	}
	glViewport(0, 0, mainCamera.windowWidth, mainCamera.windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/**********************************/

	//Shader testShader((char *)"VS CODE\n", (char *)"FS CODE\n");
	//testShader.createShaderProgram();

	
	
	/******* SET CALLBACKS *******/
	glfwSetKeyCallback(window, keyCallback);
	/*****************************/




	/******* BOX2D SETUP *******/
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	/***************************/

	//TEST
	std::vector<b2Vec2> lala;
	lala.push_back(b2Vec2(0.0f, 0.0f));
	Body testBody(&world);
	Shader bodyShader(fishVS, fishFS);
	//testBody.phisicalBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);
	testBody.init(16, bodyShader);
	getchar();
	//glDrawArrays(GL_LINE)

	/******* MAIN LOOP *******/
	for (int32 i = 0; i < 100; i++)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();
		std::cout << "x: " << position.x << "\ty: " << position.y << "\ttheta: " << angle << std::endl;
	}
	
	clock_t currentTime, previousTime;
	currentTime = previousTime = clock();
	GLfloat timeInterval = 0.0f;
	GLfloat period = 1000.0f / fps;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		currentTime	 = clock();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timeInterval += 1000.0 * (currentTime - previousTime) / CLOCKS_PER_SEC;
		while (timeInterval >= period) 
		{
			std::cout << "Time: " << timeInterval << std::endl; //TEST
			timeInterval -= period;
			testBody.draw(&mainCamera);//TODO CHANGE TO VECTOR
			glfwSwapBuffers(window);
		}
		//std::cout << 1000.0 * (clock() - timeInterval) / CLOCKS_PER_SEC << std::endl;
		//for(int a = 0; a < 1000000; a++){}
		//timeInterval = clock();

		glBindVertexArray(0);
		//glfwSwapBuffers(window);

		previousTime = currentTime;
	}
	/*************************/

	return 0;
}