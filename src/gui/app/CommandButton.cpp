#include "CommandButton.h"
#include "Definitions.h"

CommandButton::CommandButton(const sf::String& instruction, unsigned numberOfInputs, const sf::Font& font,
                             const sf::String& runTexturePath) :
	inputFields(numberOfInputs),
	runButton_(runTexturePath)
{
	/* Label init */
	buttonLabel_.setFont(font);
	buttonLabel_.setFillColor(sf::Color::White);
	buttonLabel_.setCharacterSize(24);
	buttonLabel_.setString(instruction);

	/* Background init */
	background_.setFillColor(sf::Color::Black);
	setSize_(numberOfInputs);
	setPosition(sf::Vector2f(0, 0));
}

bool CommandButton::containsRunButton(const sf::Vector2f& point) const
{
	if (runButton_.sprite.getGlobalBounds().contains(point)) return true;
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

	const auto len = inputFields.begin()->getLength();

	for (int i = 0; i < inputFields.size(); i++)
	{
		inputFields[i].setPosition(sf::Vector2f(
			position.x + 20.0,
			buttonLabel_.getPosition().y + buttonLabel_.getGlobalBounds().height
			+ BUTTON_LABEL_TOP_INDENT * (i + 1) + len * i
		));
	}

	runButton_.sprite.setPosition(sf::Vector2f(
		background_.getPosition().x + background_.getGlobalBounds().width
		- runButton_.sprite.getGlobalBounds().width * 1.5,
		background_.getPosition().y + background_.getGlobalBounds().height / 2
		- runButton_.sprite.getGlobalBounds().height / 2
	));
}

void CommandButton::drawTo(sf::RenderWindow& window) const
{
	window.draw(background_);
	window.draw(buttonLabel_);
	window.draw(runButton_.sprite);
	for (const auto& field : inputFields) field.drawTo(window);
}

void CommandButton::setSize_(const unsigned amount)
{
	const auto len = inputFields.begin()->getLength();

	background_.setSize(sf::Vector2f(
		300,
		buttonLabel_.getGlobalBounds().height * 2.0 + (len + BUTTON_LABEL_TOP_INDENT) * amount
	));
}

CommandButton::RunButton::RunButton(const sf::String& texturePath)
{
	texture.loadFromFile(texturePath);
	sprite.setTexture(texture);
}
