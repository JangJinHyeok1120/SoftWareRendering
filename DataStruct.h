#pragma once
#include <list>

using namespace std;

struct Vertex3
{
	Vertex3(float X, float Y, float Z) :x(X), y(Y), z(Z) {};

	float x;
	float y;
	float z;
};

class Mesh
{
public:
	list<Vertex3> VertexList;
	list<int> IndexList;

};
