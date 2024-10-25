#include "pch.h"
#include "Edge.h"
Edge::Edge(Vertical vert){
}
void Edge::AddConnection(Vertical vert) {
	verts[1] = vert;
}
void Edge::ReWriteFirst(Vertical vert) {
	verts[0] = vert;
}