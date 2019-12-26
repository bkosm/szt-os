#pragma once
#include "InputField.h"
#include <SFML/Graphics.hpp>

class CommandButton
{
public:
	CommandButton(const sf::String& instruction, unsigned numberOfInputs, const sf::Font& font, const sf::Texture& texture);
	~CommandButton() = default;

	bool containsRunButton(const sf::Vector2f& point) const;

	void toggleInputFieldFocus(InputField& field);
	void writeCommand(const sf::String& string);
	void setPosition(const sf::Vector2f& position);
	void drawTo(sf::RenderWindow& window) const;
	void disableFocus();
	
	std::string getInstruction();
	
	void markRunButtonReleased();
	void markRunButtonPressed();

	bool draw{false};
	std::vector<InputField> inputFields;
	
private:
	void adjustBackgroundSize_();
	
	sf::Text buttonLabel_;
	sf::RectangleShape background_;

	sf::Sprite runButton_;
};