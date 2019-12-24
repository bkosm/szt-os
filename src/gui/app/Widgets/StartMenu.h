#pragma once
#include "Button.h"
#include <SFML/Graphics.hpp>

class StartMenu
{
public:
	StartMenu();
	~StartMenu() = default;

	const sf::Sprite& getLogo() const;
	const sf::Sprite& getTermIcon() const;
	const Button& getButton() const;

	bool toggleTermButtonState();

	bool isMenuDrawn() const;

	void drawMenuButton();
	void hideMenuButton();

	void drawTo(sf::RenderWindow& window) const;
	void setFont(const sf::Font& font);

private:
	void iconSetup_();

	Button shutdown_;

	sf::Sprite logo_;
	sf::Texture logoTexture_;

	sf::Sprite termIcon_;
	sf::Texture termTexture_;
	bool termPressed_{false};

	sf::RectangleShape bar_;
};
