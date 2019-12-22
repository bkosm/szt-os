#include "../Definitions.h"
#include "Taskbar.h"

Taskbar::Taskbar(sf::RenderWindow& window) :
	window_(window)
{
	bar_ = sf::RectangleShape(sf::Vector2f(RESOLUTION.width, RESOLUTION.height / 16));
	bar_.setFillColor(sf::Color::Black);

	bar_.setPosition(0.0, float(RESOLUTION.height * 15 / 16));
}

bool Taskbar::containsStartButton(const sf::Vector2f& point) const
{
	if (menu_.getLogo().getGlobalBounds().contains(point)) return true;
	return false;
}

bool Taskbar::containsShutdownButton(const sf::Vector2f& point) const
{
	if (menu_.getButton().getGlobalBounds().contains(point) and menu_.isMenuDrawn()) return true;
	return false;
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

void Taskbar::drawTaskbar()
{
	window_.draw(bar_);
	dateTime_.updateAndDrawTo(window_);
	menu_.drawTo(window_);
}

void Taskbar::setFont(const sf::Font& font)
{
	dateTime_.setFont(font);
	menu_.setFont(font);
}
