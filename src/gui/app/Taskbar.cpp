#include "Taskbar.h"
#include "Definitions.h"

Taskbar::Taskbar(sf::RenderWindow& window) :
	window_(window)
{
	bar_ = sf::RectangleShape(sf::Vector2f(RESOLUTION.width, RESOLUTION.height / 16));
	bar_.setFillColor(sf::Color::Black);

	bar_.setPosition(0.0, float(RESOLUTION.height * 15 / 16));
}

void Taskbar::draw()
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
