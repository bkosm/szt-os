#pragma once
#include <SFML/Graphics.hpp>

class InputField
{
public:
	InputField();
	~InputField() = default;

	bool contains(const sf::Vector2f& point) const;

	bool operator==(const InputField& other) const;

	void setLabelAndFont(const sf::String& label, const sf::Font& font);
	void writeText(const sf::String& string);
	void setFocused();
	void setUnfocused();
	void setPosition(const sf::Vector2f& position);
	void drawTo(sf::RenderWindow& window) const;

	sf::String getInput() const;
	float getLength() const;

	bool isFocused{false};

private:
	sf::RectangleShape background_;
	sf::Text label_;

	sf::Text enteredText_;
	sf::String inputString_;
	sf::String visibleText_;
};
