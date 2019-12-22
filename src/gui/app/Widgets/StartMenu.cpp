#include "../Definitions.h"
#include "StartMenu.h"

StartMenu::StartMenu() :
	shutdown_(0, 0, sf::Vector2f(300, 100), sf::String("Power off"))
{
	logoTexture_.loadFromFile(LOGO_PATH);
	logoTexture_.setSmooth(true);

	shutdown_.draw = false;
	shutdown_.setTemporaryColor(sf::Color(50, 50, 50));
	shutdown_.setPosition(sf::Vector2f(shutdown_.getGlobalBounds().width / 2,
	                      RESOLUTION.height * 15 / 16 - shutdown_.getGlobalBounds().height / 2));

	logo_.setTexture(logoTexture_);

	const auto scale = RESOLUTION.height / 16 / logo_.getGlobalBounds().height;
	logo_.setScale(scale, scale);

	logo_.setOrigin(logo_.getGlobalBounds().width / 2, logo_.getGlobalBounds().height / 2);
	logo_.setPosition(RESOLUTION.width * 1 / 100, RESOLUTION.height * 19 / 20);
}

const sf::Sprite& StartMenu::getLogo() const
{
	return logo_;
}

const Button& StartMenu::getButton() const
{
	return shutdown_;
}

bool StartMenu::isMenuDrawn() const
{
	return shutdown_.draw;
}

void StartMenu::drawMenuButton()
{
	shutdown_.draw = true;
}

void StartMenu::hideMenuButton()
{
	shutdown_.draw = false;
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
