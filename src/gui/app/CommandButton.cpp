#include "CommandButton.h"
#include "Definitions.h"

CommandButton::CommandButton(const sf::String& instruction, unsigned numberOfInputs, const sf::Font& font,
	const sf::Texture& texture) :
	inputFields(numberOfInputs)
{
	runButton_.setTexture(texture);

	/* Label init */
	buttonLabel_.setFont(font);
	buttonLabel_.setFillColor(sf::Color::White);
	buttonLabel_.setCharacterSize(24);
	buttonLabel_.setString(instruction);

	/* Background init */
	background_.setFillColor(sf::Color::Black);
	adjustBackgroundSize_();
	setPosition(sf::Vector2f(0, 0));
}

bool CommandButton::containsRunButton(const sf::Vector2f& point) const
{
	if (runButton_.getGlobalBounds().contains(point)) return true;
	return false;
}

void CommandButton::toggleInputFieldFocus(InputField& field)
{
	for (auto& elem : inputFields)
	{
		if (elem == field)
		{
			elem.setFocused();
		}
		else
		{
			elem.setUnfocused();
		}
	}
}

void CommandButton::writeCommand(const sf::String& string)
{
	for (auto& field : inputFields)
	{
		if (field.isFocused)
		{
			field.writeText(string);
			return;
		}
	}
}

void CommandButton::setPosition(const sf::Vector2f& position)
{
	background_.setPosition(position);
	buttonLabel_.setPosition(
		position.x + background_.getGlobalBounds().width / 2 - buttonLabel_.getGlobalBounds().width / 2,
		position.y + BUTTON_LABEL_TOP_INDENT);

	float len = buttonLabel_.getGlobalBounds().height * 3.0;
	if (!inputFields.empty()) len = inputFields.begin()->getLength();

	for (int i = 0; i < inputFields.size(); i++)
	{
		inputFields[i].setPosition(sf::Vector2f(
			position.x + 10.0,
			position.y + BUTTON_LABEL_TOP_INDENT * 3 + buttonLabel_.getGlobalBounds().height
			+ (len + BUTTON_LABEL_TOP_INDENT) * i
		));
	}

	if (inputFields.empty()) {
		runButton_.setPosition(sf::Vector2f(
			background_.getPosition().x + background_.getGlobalBounds().width / 2.0 - runButton_.getGlobalBounds().width / 2.0,
			background_.getPosition().y + background_.getGlobalBounds().height / 2 - runButton_.getGlobalBounds().height / 5.0
		));
	}
	else {
		runButton_.setPosition(sf::Vector2f(
			background_.getPosition().x + background_.getGlobalBounds().width
			- 40 - runButton_.getGlobalBounds().width / 2.0,
			background_.getPosition().y + background_.getGlobalBounds().height / 2
			- runButton_.getGlobalBounds().height / 2.0
		));
	}
}

void CommandButton::drawTo(sf::RenderWindow& window) const
{
	if (draw)
	{
		window.draw(background_);
		window.draw(buttonLabel_);
		window.draw(runButton_);

		for (const auto& field : inputFields) field.drawTo(window);
	}
}

void CommandButton::disableFocus()
{
	for (auto& field : inputFields)
	{
		field.setUnfocused();
	}
}

std::string CommandButton::getInstruction()
{
	return buttonLabel_.getString().toAnsiString();
}

void CommandButton::markRunButtonReleased()
{
	runButton_.setColor(sf::Color(255, 255, 255, 255));
}

void CommandButton::markRunButtonPressed()
{
	runButton_.setColor(sf::Color(255, 255, 255, 30));
}

void CommandButton::adjustBackgroundSize_()
{
	if (!inputFields.empty()) {
		background_.setSize(sf::Vector2f(
			300,
			BUTTON_LABEL_TOP_INDENT * 2.0 + buttonLabel_.getGlobalBounds().height * 2.0
			+ (BUTTON_LABEL_TOP_INDENT + inputFields.begin()->getLength()) * inputFields.size()
		));
	}
	else {
		background_.setSize(sf::Vector2f(
			300,
			BUTTON_LABEL_TOP_INDENT * 2.0 + buttonLabel_.getGlobalBounds().height * 2.0
			+ (BUTTON_LABEL_TOP_INDENT + buttonLabel_.getGlobalBounds().height * 2.0)
		));
	}
}
