#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button(float x, float y, const sf::Vector2f& size, const sf::String& title);
	virtual ~Button() = default;

	bool draw{ false };
	bool clicked{ false };

	virtual sf::FloatRect getGlobalBounds() const;

	virtual void markAsClicked();
	virtual void markAsReleased();

	virtual void setTemporaryColor(const sf::Color& color);
	virtual void drawTo(sf::RenderWindow& window) const;
	virtual void setPosition(const sf::Vector2f& position);
	virtual void setLabelFont(const sf::Font& font);

private:
	sf::RectangleShape shape_;
	sf::Text label_;

	virtual void setRelativeLabelPosition_();
};
