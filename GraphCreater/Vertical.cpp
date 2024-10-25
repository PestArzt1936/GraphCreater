#include "pch.h"
#include "Vertical.h"
Vertical::Vertical(std::string name,int x,int y) {
	this->name = name;
	this->x = x;
	this->y = y;
}
void Vertical::ChangeName(std::string name) {
	this->name = name;
}
void Vertical::ChangeCords(int x, int y) {
	this->x = x;
	this->y = y;
}
/*Vertical Vertical::operator=(Vertical vert) {

}*/