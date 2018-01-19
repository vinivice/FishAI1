#include"body.h"

std::vector<b2Vec2> Body::bodyVertices;

Body::Body(GLint bodyResolution)
{
	if (this->bodyVertices.empty())
	{
		std::cout << "EMPTY";
		this->bodyVertices.push_back(b2Vec2(0.0f, 0.0f));
	}
}
