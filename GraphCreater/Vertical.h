#pragma once
#include <iostream>
class Vertical
{
	std::string name;
	
public:
	int x, y;
	Vertical() {};
	Vertical(std::string,int x,int y);
	void ChangeName(std::string);
	void ChangeCords(int x,int y);
	std::string GetName();
	//Vertical operator=(Vertical vert);
};

