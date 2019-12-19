#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Button
{
public:
	double x;
	double y;
	double width;
	double height;
	//std::string title_;
	sf::Font font;
	void drawTo(sf::RenderWindow& window);
//public:
	Button(double x, double y, double width, double height, std::string title);
	Button();
	~Button();

	sf::Text buttonTitle;
	sf::RectangleShape buttonShape;
};

