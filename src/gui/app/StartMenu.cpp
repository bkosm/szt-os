#include "StartMenu.h"
#include "Definitions.h"

StartMenu::StartMenu() :
	shutdown_(0, 0, sf::Vector2f(300, 400), sf::String("Power off"))
{
	logoTexture_.loadFromFile(LOGO_PATH);
	logoTexture_.setSmooth(true);

	shutdown_.draw = false;

	logo_.setTexture(logoTexture_);

	const auto scale = RESOLUTION.height / 16 / logo_.getGlobalBounds().height;
	logo_.setScale(scale, scale);

	logo_.setOrigin(logo_.getGlobalBounds().width / 2, logo_.getGlobalBounds().height / 2);
	logo_.setPosition(RESOLUTION.width * 1 / 100, RESOLUTION.height * 19 / 20);
}

void StartMenu::drawTo(sf::RenderWindow& window) const
{
	window.draw(logo_);
	if (shutdown_.draw) shutdown_.drawTo(window);
}

void StartMenu::setFont(const sf::Font& font)
{
	shutdown_.setLabelFont(font);
}
