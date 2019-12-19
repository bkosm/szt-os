#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class CreateWindow
{
private:
	double x;
	double y;
	double width;
	double height;
	
public:
	CreateWindow(double x, double y, double width, double height, std::string n1);
	//CreateWindow(double x, double y, double width, double height, std::string n1, std::string n2);
	CreateWindow();
	~CreateWindow();

	sf::RectangleShape CWShape;
	sf::RectangleShape write;
	
};

