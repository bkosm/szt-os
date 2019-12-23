#include "Button.h"
#include <SFML/Graphics.hpp>
#include "../Definitions.h"

Button::Button(float x, float y, const sf::Vector2f& size, const sf::String& title)
{
	label_.setString(title);
	label_.setCharacterSize(24);

	shape_ = sf::RectangleShape(size);
	markAsReleased();

	shape_.setPosition(sf::Vector2f(x, y));
}

sf::FloatRect Button::getGlobalBounds() const
{
	return shape_.getGlobalBounds();
}

void Button::markAsClicked()
{
	shape_.setFillColor(sf::Color(0, 0, 0, 50));
	label_.setFillColor(sf::Color(255, 255, 255, 50));

	clicked = true;
}

void Button::markAsReleased()
{
	shape_.setFillColor(sf::Color(0, 0, 0));
	label_.setFillColor(sf::Color(255, 255, 255));

	clicked = false;
}

void Button::setTemporaryColor(const sf::Color& color)
{
	shape_.setFillColor(color);
}

void Button::drawTo(sf::RenderWindow& window) const
{
	window.draw(shape_);
	window.draw(label_);
}

void Button::setPosition(const sf::Vector2f& position)
{
	shape_.setPosition(position);
	setRelativeLabelPosition_();
}

void Button::setLabelFont(const sf::Font& font)
{
	label_.setFont(font);
	shape_.setOrigin(sf::Vector2f(shape_.getGlobalBounds().width / 2, shape_.getGlobalBounds().height / 2));
	setRelativeLabelPosition_();
}

void Button::setRelativeLabelPosition_()
{
	const auto shapePos = shape_.getGlobalBounds();
	const auto labelSize = label_.getGlobalBounds();
	const auto labelIndent = (shapePos.width - labelSize.width) / 2.0;

	label_.setPosition(sf::Vector2f(shapePos.left + labelIndent,
	                                shapePos.top + shapePos.height / 2 - labelSize.height / 2));
}
