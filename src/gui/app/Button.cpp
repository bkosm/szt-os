#include "Button.h"
#include "Definitions.h"
#include <SFML/Graphics.hpp>
#include "App.h"

void Button::drawTo(sf::RenderWindow& window) const
{
	window.draw(shape_);
	window.draw(label_);
}

void Button::setPosition(sf::Vector2f position)
{
	shape_.setPosition(position);
	setRelativeLabelPosition_();
}

Button::Button(float x, float y, const sf::Vector2f& size, const sf::String& title, std::shared_ptr<AppData> appData) :
	data_(std::move(appData))
{
	label_.setString(title);
	label_.setFillColor(sf::Color::White);
	label_.setCharacterSize(24);
	label_.setFont(data_->resources.appFont);

	shape_ = sf::RectangleShape(size);
	shape_.setFillColor(sf::Color::Black);

	shape_.setOrigin(sf::Vector2f(shape_.getGlobalBounds().width / 2, shape_.getGlobalBounds().height / 2));
	shape_.setPosition(sf::Vector2f(x, y));
	setRelativeLabelPosition_();
}

void Button::setRelativeLabelPosition_()
{
	const auto shapePos = shape_.getGlobalBounds();
	const auto labelSize = label_.getGlobalBounds();
	const auto labelIndent = (shapePos.width - labelSize.width) / 2.0;

	label_.setPosition(sf::Vector2f(shapePos.left + labelIndent,
	                                shapePos.top + BUTTON_LABEL_TOP_INDENT));
}
