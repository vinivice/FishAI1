#include "seed.h"

Shader Seed::shader;
GLuint Seed::VAO;
GLuint Seed::numberVertices;
std::default_random_engine* Seed::generator;

Seed::Seed(b2World* world, GLubyte* chromossomeInput, GLushort category, GLfloat px, GLfloat py, GLfloat vx, GLfloat vy, GLfloat ttl)
{
	int i;
	//TODO uncomment
	/*for (i = 0; i < CHROMOSSOMES_SIZE; i++)
	{
		this->chromossome[i] = chromossomeInput[i];
	}*/

	b2BodyDef seedDef;
	seedDef.type = b2_dynamicBody;
	seedDef.position.Set(px, py);
	seedDef.linearVelocity.Set(vx, vy);
	seedDef.userData = this;
	seedDef.linearDamping = LINEAR_DAMPING;
	seedDef.angularDamping = ANGULAR_DAMPING;
	this->phisicalBody = world->CreateBody(&seedDef);

	b2CircleShape seedShape;
	seedShape.m_p.SetZero();

	b2FixtureDef seedFixtureDef;
	seedFixtureDef.shape = &seedShape;

	seedShape.m_radius = SEED_RADIUS;
	seedFixtureDef.density = 0.5f;


    this->category = category;

	if (category == SEX_SEED_CATEGORY)
	{
		seedFixtureDef.filter.categoryBits = SEX_SEED_CATEGORY;
		seedFixtureDef.filter.maskBits = FISH_BODY_CATEGORY | RING_CATEGORY;
		
		this->color[0] = 0.0f;
		this->color[1] = 0.0f;
		this->color[2] = 1.0f;

		//std::cout << "SEX\n";
	}
	else
	{
		seedFixtureDef.filter.categoryBits = ASEX_SEED_CATEGORY;
		seedFixtureDef.filter.maskBits = RING_CATEGORY;

		this->color[0] = 1.0f;
		this->color[1] = 1.0f;
		this->color[2] = 0.0f;


		//std::cout << "ASEX\n";
	}
	this->phisicalBody->CreateFixture(&seedFixtureDef);
    this->ttl = ttl;

	
}

Seed::~Seed()
{
	this->phisicalBody->GetWorld()->DestroyBody(this->phisicalBody);
    //std::cout << this << " DESTROIED\n";
}

bool Seed::init(GLuint resolution, Shader shaderInput, std::default_random_engine *generatorInput)
{
	shader = shaderInput;
	shader.createShaderProgram();

	generator = generatorInput;

	std::vector<GLfloat> vertices;

	GLfloat theta = 2 * b2_pi / resolution;
	GLfloat sinValue = std::sin(theta);
	GLfloat cosValue = std::cos(theta);

	GLfloat x = 1.0f, y = 0.0f, x2, y2;

	vertices.push_back(0.0f);
	vertices.push_back(0.0f);

	//Vertices and indices 
	int k;
	for (k = 0; k < resolution; k++)
	{
		vertices.push_back(x);
		vertices.push_back(y);

		x2 = cosValue * x - sinValue * y;
		y2 = sinValue * x + cosValue * y;
		x = x2;
		y = y2;
	}

	vertices.push_back(1.0f);
	vertices.push_back(0.0f);

	numberVertices = vertices.size() / 2;

	GLuint VBO;
	GLfloat *v = vertices.data();
	GLuint vSize = vertices.size() * sizeof(GLfloat);

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, v, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Clean up
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void Seed::draw(Camera *camera)
{
	b2Transform transform = this->phisicalBody->GetTransform();

	GLfloat model[16];
	GLfloat r = SEED_RADIUS;

	//std::cout << r << ", " << transform.q.c << ", " << transform.p.x << ", " << transform.p.y << "\n";

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

	//Draw seed
	glUseProgram(this->shader.shaderProgram);

	if (this->phisicalBody->IsAwake() == false)
	{
		//std::cout << "FALSE\n";
		GLfloat color[3] = { 1.0, 0.0, 0.0 };
		glUniform3fv(1, 1, color);
	}
	else
	{
		//std::cout << "TRUE\n";
		glUniform3fv(1, 1, this->color);
	}
	/*if (this->phisicalBody->GetFixtureList()[0].GetFilterData().categoryBits == ASEX_SEED_CATEGORY)
	{
		std::cout << this->phisicalBody->GetLinearVelocity().Length() << std::endl;
	}*/

	glUniformMatrix4fv(2, 1, GL_FALSE, model);
	glUniformMatrix4fv(3, 1, GL_FALSE, camera->projectionMatrix);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, this->numberVertices);

	//clean up
	glUseProgram(0);
}

bool Seed::update(GLfloat timeInterval, std::vector<Body *> *fishes)
{
    this->ttl -= timeInterval;
    bool ttlZero = ttl <= 0;
    if(ttlZero)
    {
        if(this->category == SEX_SEED_CATEGORY)
        {
            //JUST DIE? PROBABLY
            //BECOME FOOD? MAYBE
            std::cout << "S DEAD\n";
        }
        else
        {
        	b2Transform transform = this->phisicalBody->GetTransform();
            //CREATE NEW FISH
            //TODO change random fish for specific fish
            fishes->push_back(new Body(this->phisicalBody->GetWorld(), transform.p.x, transform.p.y, transform.q.GetAngle()));
            std::cout << "AS DEAD\n";
        }
    }

    return ttlZero;
}

