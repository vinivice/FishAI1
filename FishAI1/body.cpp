#include"body.h"
#include <cmath>

std::vector<b2Vec2> Body::bodyVertices;

bool Body::structureIsCreated = false;
const GLfloat Body::borderSize = 0.1f;

//void Body::createStructure(GLint bodyResolution, GLfloat *v, GLuint *i)
void Body::createStructure(GLint bodyResolution, std::vector<GLfloat> &vertices, std::vector<GLuint> &indices)
{
	if (this->structureIsCreated == false)
	{
		std::cout << "FALSE\n"; //TODO DELETE
		this->structureIsCreated = true;

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

		vertices.push_back(0.0f); 
		vertices.push_back(0.0f);
		vertices.push_back(0.8 * innerRadius / 2.0f);
		vertices.push_back(0.8 * innerRadius * sqrt(3.0f) / 2.0f);
		vertices.push_back(-0.8 * innerRadius / 2.0f);
		vertices.push_back(0.8 * innerRadius * sqrt(3.0f) / 2.0f);

		indices.push_back(2 * bodyResolution);
		indices.push_back(2 * bodyResolution + 1);
		indices.push_back(2 * bodyResolution + 2);

		//TODO DELETE
		/*v = vertices.data();
		for (int j = 0; j < vertices.size(); j++)
		{
			std::cout << v[j] << std::endl;
		}
		i = indices.data();
		for (int j = 0; j < indices.size(); j+=3)
		{
			std::cout << i[j] << ", " << i[j+1] << ", " << i[j+2] << std::endl;
		}*/
		getchar();	
	}
	else
	{
		std::cout << "TRUE\n";
	}
}

void Body::init(GLint bodyResolution)
{
	//GLfloat *vertices = NULL;
	//GLuint *indices = NULL;
	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	this->createStructure(bodyResolution, vertices, indices);

	for (int j = 0; j < sizeof(indices)/sizeof(GLuint); j += 3)
	{
		std::cout << indices[j] << ", " << indices[j + 1] << ", " << indices[j + 2] << std::endl;
	}
	getchar();

	//TODO Check. Probably not needed.
	if (this->bodyVertices.empty())
	{
		std::cout << "EMPTY\n";
		this->bodyVertices.push_back(b2Vec2(0.0f, 0.0f));
	}
	else
	{
		std::cout << "NOT EMPTY\n";
	}
}
