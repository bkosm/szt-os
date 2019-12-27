#include "InputField.h"
#include "Definitions.h"

InputField::InputField()
{
	/* Label init */
	label_.setFillColor(sf::Color::White);
	label_.setCharacterSize(16);

	/* Text entered init */
	enteredText_.setFillColor(sf::Color::Black);
	enteredText_.setCharacterSize(16);

	/* Background init */
	background_.setSize(sf::Vector2f(200.0, 20.0));
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
		if (string.getData()[0] == 8 and inputString_.getSize() > 0)
		{
			inputString_.erase(inputString_.getSize() - 1);
		}
			/* handle backspace and newline chars */
		else if (string.getData()[0] != 8 and string.getData()[0] != 13)
		{
			inputString_ += string;
		}

		if (inputString_.getSize() > 16)
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
	                        position.y + INPUT_FIELD_LABEL_HEIGHT * 2);
	enteredText_.setPosition(position.x + 2,
	                         position.y + INPUT_FIELD_LABEL_HEIGHT * 2);
}

void InputField::drawTo(sf::RenderWindow& window) const
{
	window.draw(label_);
	window.draw(background_);
	window.draw(enteredText_);
}

sf::String InputField::getInput()
{
	const auto out = inputString_;

	inputString_ = "";
	visibleText_ = "";
	enteredText_.setString(visibleText_);

	return out;
}

float InputField::getLength() const
{
	return background_.getGlobalBounds().height + INPUT_FIELD_LABEL_HEIGHT * 2;
}
