#include "../Definitions.h"
#include "StartMenu.h"

StartMenu::StartMenu() :
	shutdown_(0, 0, sf::Vector2f(300, 100), sf::String("Power off")),
	menu_(0, 0, sf::Vector2f(300, 100), sf::String("Menu"))
{
	iconSetup_();

	bar_ = sf::RectangleShape(sf::Vector2f(2.5, RESOLUTION.height / 32.0 + 2.5));
	bar_.setFillColor(sf::Color::White);
	bar_.setPosition(RESOLUTION.width * 54 / 1000, RESOLUTION.height * 19 / 20);

	shutdown_.draw = false;
	shutdown_.setTemporaryColor(sf::Color(50, 50, 50));
	shutdown_.setPosition(sf::Vector2f(shutdown_.getGlobalBounds().width / 2,
		RESOLUTION.height * 15 / 16 - shutdown_.getGlobalBounds().height / 2));

	menu_.draw = false;
	menu_.setTemporaryColor(sf::Color(50, 50, 50));
	menu_.setPosition(sf::Vector2f(shutdown_.getGlobalBounds().width / 2,
		RESOLUTION.height * 12.8 / 16 - shutdown_.getGlobalBounds().height / 2));
}

const sf::Sprite& StartMenu::getLogo() const
{
	return logo_;
}

const sf::Sprite& StartMenu::getTermIcon() const
{
	return termIcon_;
}

const Button& StartMenu::getShutdownButton() const
{
	return shutdown_;
}

const Button& StartMenu::getMenuButton() const
{
	return menu_;
}

bool StartMenu::toggleTermButtonState()
{
	if (termPressed_)
	{
		termIcon_.setColor(sf::Color(255, 255, 255, 255));
		termPressed_ = false;
		return false;
	}

	termIcon_.setColor(sf::Color(255, 255, 255, 30));
	termPressed_ = true;
	return true;
}

bool StartMenu::isMenuDrawn() const
{
	return shutdown_.draw;
}

void StartMenu::drawMenuButton()
{
	logo_.setColor(sf::Color(255, 255, 255, 30));
	shutdown_.draw = true;
	menu_.draw = true;
}

void StartMenu::hideMenuButton()
{
	logo_.setColor(sf::Color(255, 255, 255, 255));
	shutdown_.draw = false;
	menu_.draw = false;
}

void StartMenu::drawTo(sf::RenderWindow& window) const
{
	window.draw(logo_);
	window.draw(bar_);
	window.draw(termIcon_);
	if (shutdown_.draw) {
		shutdown_.drawTo(window);
		menu_.drawTo(window);
	}
}

void StartMenu::setFont(const sf::Font& font)
{
	shutdown_.setLabelFont(font);
	menu_.setLabelFont(font);
}

void StartMenu::iconSetup_()
{
	logoTexture_.loadFromFile(LOGO_PATH);
	logoTexture_.setSmooth(true);

	logo_.setTexture(logoTexture_);

	const auto scale = RESOLUTION.height / 16 / logo_.getGlobalBounds().height;
	logo_.setScale(scale, scale);

	logo_.setOrigin(logo_.getGlobalBounds().width / 2, logo_.getGlobalBounds().height / 2);
	logo_.setPosition(RESOLUTION.width * 1 / 100, RESOLUTION.height * 19 / 20);

	termTexture_.loadFromFile(TERM_ICON_PATH);
	termTexture_.setSmooth(true);

	termIcon_.setTexture(termTexture_);

	termIcon_.setScale(scale, scale);

	termIcon_.setOrigin(termIcon_.getGlobalBounds().width / 2, termIcon_.getGlobalBounds().height / 2);
	termIcon_.setPosition(RESOLUTION.width * 8 / 100, RESOLUTION.height * 19 / 20);
}
