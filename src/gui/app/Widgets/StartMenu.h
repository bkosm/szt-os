#pragma once
#include "Button.h"
#include <SFML/Graphics.hpp>

class StartMenu
{
public:
	StartMenu();
	~StartMenu() = default;

	const sf::Sprite& getLogo() const;
	const Button& getButton() const;

	bool isMenuDrawn() const;
	
	void drawMenuButton();
	void hideMenuButton();
	
	void drawTo(sf::RenderWindow& window) const;
	void setFont(const sf::Font& font);

private:
	Button shutdown_;
	sf::Sprite logo_;
	sf::Texture logoTexture_;
};
