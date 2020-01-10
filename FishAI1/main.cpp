#include<iostream>

#if OS == linux
    #include<GL/glew.h>
    #include<GLFW/glfw3.h>
    #include<Box2D/Box2D.h>
#else
    #include<GL\glew.h>
    #include<GLFW\glfw3.h>
    #include<Box2D\Box2D.h>
#endif

#include<vector>
#include<ctime>

#include"graphics.h"
#include"body.h"
#include"shader_codes.h"
#include "ring.h"
#include "sensor.h"
#include "seed.h"


bool pause = true;
bool lForce = false;
bool rForce = false;
bool drawSensors = false;
/******* CALLBACK FUNCTIONS *******/
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_I:
		if (action != GLFW_RELEASE)
		{
			mainCamera.zoomIn();
		}
		break;
	case GLFW_KEY_O:
		if (action != GLFW_RELEASE)
		{
			mainCamera.zoomOut();
		}
		break;
	case GLFW_KEY_W:
		if (action != GLFW_RELEASE)
		{
			mainCamera.move(0.0f, 1.0f);
		}
		break;
	case GLFW_KEY_S:
		if (action != GLFW_RELEASE)
		{
			mainCamera.move(0.0f, -1.0f);
		}
		break;
	case GLFW_KEY_A:
		if (action != GLFW_RELEASE)
		{
			mainCamera.move(-1.0f, 0.0f);
		}
		break;
	case GLFW_KEY_D:
		if (action != GLFW_RELEASE)
		{
			mainCamera.move(1.0f, 0.0f);
		}
		break;
	case GLFW_KEY_P:
		if (action == GLFW_PRESS)
		{
			pause = !pause;
			if (pause)
			{
				glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			}
			else
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
		break;
	case GLFW_KEY_N:
		if (action == GLFW_PRESS)
		{
			lForce = !lForce;
		}
		break;
	case GLFW_KEY_M:
		if (action == GLFW_PRESS)
		{
			rForce = !rForce;
		}
		break;
	case GLFW_KEY_J:
		if (action == GLFW_PRESS)
		{
			rForce = !rForce;
			lForce = !lForce;
		}
		break;
	case GLFW_KEY_E:
		if (action == GLFW_PRESS)
		{
			drawSensors = !drawSensors;
		}
		break;

	}
}
/**********************************/

int main(int argc, char* argv[])
{
	GLuint fps = 120;
	std::default_random_engine generator(time(0));

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
    #ifdef DEBUG
        std::cout << "WINDOW OK\n";
    #endif

	glfwMakeContextCurrent(window);

	//-----------
	
	glewExperimental = GL_TRUE; //TODO Check what it does
	
	if(GLEW_OK != glewInit())
	{
		std::cout << "GLEW FAIL\n";
		return EXIT_FAILURE;
	}

    #ifdef DEBUG
        std::cout << "GLEW OK\n";
    #endif


	glViewport(0, 0, mainCamera.windowWidth, mainCamera.windowHeight);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	/**********************************/

	//Shader testShader((char *)"VS CODE\n", (char *)"FS CODE\n");
	//testShader.createShaderProgram();

	
	
	/******* SET CALLBACKS *******/
	glfwSetKeyCallback(window, keyCallback);
	/*****************************/




	/******* BOX2D SETUP *******/
	//b2Vec2 gravity(0.0f, -10.0f);
	b2Vec2 gravity(0.0f, 0.0f);
	b2World world(gravity);

	MyContactListener* contactListener = new MyContactListener();
	world.SetContactListener(contactListener);
	/*
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);*/
	/*
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

    #ifdef DEBUG
        std::cout << "BOX2D SETUP OK\n";
    #endif



	float32 timeStep = 1.0f / 60.0f;
	*/
	/***************************/

	std::vector<Body *> fishes;
	std::vector<Seed *> seeds;
	
	//TEST
	//std::vector<b2Vec2> lala;
	//lala.push_back(b2Vec2(0.0f, 0.0f));
	//Shader bodyShader(fishVS, fishFS);
	Shader ringShader(ringVS, ringFS);
	Body::init(BODY_RESOLUTION, Shader(fishVS, fishFS), Shader(eyeVS, eyeFS), &generator);
	fishes.push_back(new Body(&world, -0.1f, 1.5f, 0.0f));
	fishes.push_back(new Body(&world, -1.2f, 0.0f, b2_pi / 2.0f));
	fishes.push_back(new Body(&world, -1.2f, -20.0f, b2_pi / 2.0f));

	Seed::init(SEED_RESOLUTION, Shader(fishVS, fishFS), &generator);
	seeds.push_back(new Seed(&world, NULL, SEX_SEED_CATEGORY, -1.0f, -10.0f, 0.0f, 0.0f));
	seeds.push_back(new Seed(&world, NULL, ASEX_SEED_CATEGORY, 1.0f, -10.0f, 10.0f, 0.0f));
	//testBody.phisicalBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);
	//fishes.back()->init(32, bodyShader);
	//getchar();
	//glDrawArrays(GL_LINE)

    #ifdef DEBUG
        std::cout << "TEST OK\n";
    #endif


	/******* MAIN LOOP *******/
	/*for (int32 i = 0; i < 100; i++)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();
		std::cout << "x: " << position.x << "\ty: " << position.y << "\ttheta: " << angle << std::endl;
	}*/
	
	Ring ring(&world, 25.0f, RING_RESOLUTION, ringShader);
	GLint i;
	clock_t currentTime, previousTime;
	currentTime = previousTime = clock();
	GLfloat timeInterval = 0.0f;

    #ifdef DEBUG
        std::cout << "PRE LOOP OK\n";
    #endif

	GLfloat period = 1.0f / fps;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	while (!glfwWindowShouldClose(window))
	{

        #ifdef DEBUG
            std::cout << "LOOP RUNNNING\n";
        #endif


		glfwPollEvents();
		currentTime	= clock();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//turn this into methods
		//values based on mind output
		if (!pause)
		{
			timeInterval += 1.0 * (currentTime - previousTime) / CLOCKS_PER_SEC; //1.0* just to cast to float. maybe future time scale?
			while (timeInterval >= period)
			{
				for (i = 0; i < fishes.size(); i++)
				{
					fishes[i]->update();
					if (lForce)
					{
						fishes[i]->useLeftPropulsor(false);
					}
					if (rForce)
					{
						fishes[i]->useRightPropulsor(false);
					}
				}
				rForce = lForce = false;
				world.Step(period, velocityIterations, positionIterations);
				//std::cout << "Time: " << timeInterval << std::endl; //TEST
				timeInterval -= period;
			}
		}
		//std::cout << 1000.0 * (clock() - timeInterval) / CLOCKS_PER_SEC << std::endl;
		//for(int a = 0; a < 1000000; a++){}
		//timeInterval = clock();
		if(drawSensors)
		{
			for (i = 0; i < fishes.size(); i++)
			{
				fishes[i]->drawSensors(&mainCamera);
			}
			for (i = 0; i < fishes.size(); i++)
			{
				fishes[i]->drawEyes(&mainCamera);
			}
		}
		for (i = 0; i < seeds.size(); i++)
		{
			seeds[i]->draw(&mainCamera);
		}
		for (i = 0; i < fishes.size(); i++)
		{
			fishes[i]->drawBody(&mainCamera);
		}
		ring.draw(&mainCamera);

		glfwSwapBuffers(window);

		glBindVertexArray(0);
		//glfwSwapBuffers(window);

		previousTime = currentTime;
	}
	/*************************/

	return 0;
}
