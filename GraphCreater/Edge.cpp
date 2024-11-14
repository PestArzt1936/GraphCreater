#include "pch.h"
#include "Edge.h"
Edge::Edge(Vertical vert){
}

void Edge::AddConnection(Vertical* vert) {
	verts[1] = vert;
}
void Edge::ReWriteFirst(Vertical* vert) {
	verts[0] = vert;
}
std::string Edge::GetName() {
	return name;
}
void Edge::ChangeName(std::string name) {
	this->name = name;
}
Edge::~Edge() {
	verts[0] = nullptr;
	verts[1] = nullptr;
}