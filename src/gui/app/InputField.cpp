#include "InputField.h"

InputField::InputField()
{
	/* Label init */
	label_.setFillColor(sf::Color::White);
	label_.setCharacterSize(12);

	/* Text entered init */
	enteredText_.setFillColor(sf::Color::Black);
	enteredText_.setCharacterSize(12);

	/* Background init */
	background_.setSize(sf::Vector2f(200.0, 14.0));
	setUnfocused();
	setPosition(sf::Vector2f(0, 0));
}

bool InputField::contains(const sf::Vector2f& point) const
{
	if (background_.getGlobalBounds().contains(point)) return true;
	return false;
}

bool InputField::operator==(const InputField& other) const
{
	if (label_.getString().toAnsiString() == other.label_.getString().toAnsiString())
		return true;
	return false;
}

void InputField::setLabelAndFont(const sf::String& label, const sf::Font& font)
{
	label_.setString(label);
	label_.setFont(font);
	
	enteredText_.setFont(font);
}

void InputField::writeText(const sf::String& string)
{
	inputString_ += string;
	if (inputString_.getSize() > 15)
	{
		visibleText_ = inputString_.substring(inputString_.getSize() - 15);
	}
	else visibleText_ = inputString_;

	enteredText_.setString(visibleText_);
}

void InputField::setFocused()
{
	background_.setFillColor(sf::Color(255, 255, 255, 255));
	isFocused = true;
}

void InputField::setUnfocused()
{
	background_.setFillColor(sf::Color(255, 255, 255, 100));
	isFocused = false;
}

void InputField::setPosition(const sf::Vector2f& position)
{
	label_.setPosition(position);
	background_.setPosition(position.x,
	                        position.y + label_.getGlobalBounds().height + 2);
	enteredText_.setPosition(position.x + 4,
	                         position.y + label_.getGlobalBounds().height + 3);
}

void InputField::drawTo(sf::RenderWindow& window) const
{
	window.draw(label_);
	window.draw(background_);
	window.draw(enteredText_);
}

sf::String InputField::getInput() const
{
	return inputString_;
}

float InputField::getLength() const
{
	return background_.getGlobalBounds().width;
}
