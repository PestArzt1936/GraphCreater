#pragma once
#include <iostream>
#include "Vertical.h"
class Edge
{
	std::string name;
public:
	~Edge();
	Vertical* verts[2];
	Edge() {};
	Edge(Vertical vert);
	void AddConnection(Vertical* vert);
	void ReWriteFirst(Vertical* vert);
	std::string GetName();
	void ChangeName(std::string name);
};

