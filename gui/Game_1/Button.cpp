#include "Button.h"
#include <SFML/Graphics.hpp>

void Button::drawTo(sf::RenderWindow& window)
{
	window.draw(buttonShape);
	window.draw(buttonTitle);
}

Button::Button(double x, double y, double width, double height, std::string title)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	buttonTitle.setString(title);
	font.loadFromFile("arial.ttf");

	this->buttonShape = sf::RectangleShape(sf::Vector2f(this->width, this->height));
	this->buttonShape.setPosition(sf::Vector2f(this->x, this->y));
	this->buttonShape.setFillColor(sf::Color::White);
	buttonTitle.setCharacterSize(24);
	buttonTitle.setFillColor(sf::Color::Black);
	sf::FloatRect textRect = buttonTitle.getLocalBounds();
	//buttonTitle.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	//buttonTitle.setPosition(sf::Vector2f((width_ / 2) + x_, (height_ / 2) + y_));
	//buttonTitle.setOrigin(buttonTitle.getLocalBounds()[0] / 2, buttonTitle.getLocalBounds()[1] / 2);
	buttonTitle.setPosition(sf::Vector2f(x, y));

	buttonTitle.setFont(font);
}

Button::Button()
{
}

Button::~Button()
{
}

//sf::RectangleShape Button::drawButton()
//{
//	sf::RectangleShape buttonShape(sf::Vector2f(width, height));
//	buttonShape.setPosition(sf::Vector2f(x, y));
//	buttonShape.setFillColor(sf::Color::White);
//	buttonTitle.setCharacterSize(24);
//	buttonTitle.setFillColor(sf::Color::Black);
//	sf::FloatRect textRect = buttonTitle.getLocalBounds();
//	//buttonTitle.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
//	//buttonTitle.setPosition(sf::Vector2f((width_ / 2) + x_, (height_ / 2) + y_));
//	//buttonTitle.setOrigin(buttonTitle.getLocalBounds()[0] / 2, buttonTitle.getLocalBounds()[1] / 2);
//	buttonTitle.setPosition(sf::Vector2f( x,  y));
//	
//	buttonTitle.setFont(font);
//	return buttonShape;
//}
