#pragma once
#include <SFML/Graphics.hpp>
#include "DateTimeWidget.h"
#include "StartMenu.h"

class Taskbar
{
public:
	Taskbar(sf::RenderWindow& window);
	~Taskbar() = default;

	bool containsStartButton(const sf::Vector2f& point) const;
	bool containsShutdownButton(const sf::Vector2f& point) const;

	void toggleMenuDraw();
	void drawTaskbar();
	void setFont(const sf::Font& font);

private:
	sf::RenderWindow& window_;

	DateTimeWidget dateTime_;
	sf::RectangleShape bar_;
	StartMenu menu_;
};

