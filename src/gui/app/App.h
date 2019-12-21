#pragma once
#include "StateMaschine.hpp"
#include "Definitions.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

struct Resources
{
	sf::Font appFont;
	sf::Sound bootSound;
	sf::Sound shutdownSound;

	Resources(const sf::String& fontPath);
};

struct AppData
{
	sf::RenderWindow window;
	StateMaschine states;
	Resources resources{FONT_PATH};
};

class App
{
public:
	App(const sf::VideoMode& videoMode, const sf::String& title);
	~App() = default;

private:
	std::shared_ptr<AppData> data_ = std::make_shared<AppData>();

	void run_() const;
};
