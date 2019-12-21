#pragma once
#include <SFML/Graphics.hpp>
#include "App.h"

class Button
{
public:
	Button(float x, float y, const sf::Vector2f& size, const sf::String& title, std::shared_ptr<AppData> appData);
	~Button() = default;
	
	void drawTo(sf::RenderWindow& window) const;
	void setPosition(sf::Vector2f position);

private:
	std::shared_ptr<AppData> data_;
	sf::RectangleShape shape_;
	sf::Text label_;

	void setRelativeLabelPosition_();
};
