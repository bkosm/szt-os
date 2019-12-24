#pragma once
#include <SFML/Graphics.hpp>
#include "DateTimeWidget.h"
#include "StartMenu.h"

class Taskbar
{
public:
	Taskbar();
	~Taskbar() = default;

	bool toggleTermButton();
	bool containsStartButton(const sf::Vector2f& point) const;
	bool containsTermButton(const sf::Vector2f& point) const;
	bool containsShutdownButton(const sf::Vector2f& point) const;

	void dontDrawMenu();
	void toggleMenuDraw();
	void drawTo(sf::RenderWindow& window);
	void setFont(const sf::Font& font);

private:
	DateTimeWidget dateTime_;
	sf::RectangleShape bar_;
	StartMenu menu_;
};

