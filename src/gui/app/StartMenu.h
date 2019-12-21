#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class StartMenu
{
public:
	StartMenu();
	~StartMenu() = default;
	
	void drawTo(sf::RenderWindow& window) const;
	void setFont(const sf::Font& font);
private:

	Button shutdown_;
	sf::Sprite logo_;
	sf::Texture logoTexture_;
};

