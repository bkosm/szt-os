#pragma once
#include "InputField.h"
#include <SFML/Graphics.hpp>

class CommandButton
{
public:
	CommandButton(const sf::String& instruction, unsigned numberOfInputs, const sf::Font& font, const sf::String& runTexturePath);
	~CommandButton() = default;

	bool containsRunButton(const sf::Vector2f& point) const;

	void toggleInputFieldFocus(InputField& field);
	void writeCommand(const sf::String& string);
	void setPosition(const sf::Vector2f& position);
	void drawTo(sf::RenderWindow& window) const;

	bool draw{false};
	std::vector<InputField> inputFields;
	
private:
	void setSize_(unsigned amount);
	
	sf::Text buttonLabel_;
	sf::RectangleShape background_;

	struct RunButton
	{
		RunButton(const sf::String& texturePath);

		sf::Texture texture;
		sf::Sprite sprite;
	} runButton_;
};