#pragma once
#include <SFML/Graphics.hpp>
#include "DateTimeWidget.h"
#include "StartMenu.h"

class Taskbar
{
public:
	Taskbar(sf::RenderWindow& window);
	~Taskbar() = default;

	void draw();
	void setFont(const sf::Font& font);

private:
	sf::RenderWindow& window_;

	DateTimeWidget dateTime_;
	sf::RectangleShape bar_;
	StartMenu menu_;
};

