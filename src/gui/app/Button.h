#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(float x, float y, const sf::Vector2f& size, const sf::String& title);
	~Button() = default;

	bool draw{ false };

	void drawTo(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	void setLabelFont(const sf::Font& font);

private:
	sf::RectangleShape shape_;
	sf::Text label_;

	void setRelativeLabelPosition_();
};
