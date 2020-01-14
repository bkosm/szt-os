#include "../Definitions.h"
#include "Taskbar.h"

Taskbar::Taskbar()
{
	bar_ = sf::RectangleShape(sf::Vector2f(RESOLUTION.width, RESOLUTION.height / 16));
	bar_.setFillColor(sf::Color::Black);

	bar_.setPosition(0.0, float(RESOLUTION.height * 15 / 16));
}

bool Taskbar::toggleTermButton()
{
	return menu_.toggleTermButtonState();
}

bool Taskbar::containsStartButton(const sf::Vector2f& point) const
{
	if (menu_.getLogo().getGlobalBounds().contains(point)) return true;
	return false;
}

bool Taskbar::containsTermButton(const sf::Vector2f& point) const
{
	if (menu_.getTermIcon().getGlobalBounds().contains(point)) return true;
	return false;
}

bool Taskbar::containsShutdownButton(const sf::Vector2f& point) const
{
	if (menu_.getShutdownButton().getGlobalBounds().contains(point) and menu_.isMenuDrawn()) return true;
	return false;
}

bool Taskbar::containsMenuButton(const sf::Vector2f& point) const
{
	if (menu_.getMenuButton().getGlobalBounds().contains(point) and menu_.isMenuDrawn()) return true;
	return false;
}

void Taskbar::dontDrawMenu()
{
	menu_.hideMenuButton();
}

void Taskbar::toggleMenuDraw()
{
	if (menu_.isMenuDrawn())
	{
		menu_.hideMenuButton();
	}
	else
	{
		menu_.drawMenuButton();
	}
}

void Taskbar::drawTo(sf::RenderWindow& window)
{
	window.draw(bar_);
	dateTime_.updateAndDrawTo(window);
	menu_.drawTo(window);
}

void Taskbar::setFont(const sf::Font& font)
{
	dateTime_.setFont(font);
	menu_.setFont(font);
}
