#include "CreateWindow.h"
#include <SFML/Graphics.hpp>

CreateWindow::CreateWindow(double x, double y, double width, double height, std::string n1)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	
	this->CWShape = sf::RectangleShape(sf::Vector2f(this->width, this->height));
	this->CWShape.setPosition(x, y);
	this->CWShape.setFillColor(sf::Color::White);
	
	write = sf::RectangleShape (sf::Vector2f(100, 30));
	write.setPosition(x + width / 2 - 50, y + height / 2);
	write.setFillColor(sf::Color::Blue);
}

CreateWindow::CreateWindow()
{
}

CreateWindow::~CreateWindow()
{
}
