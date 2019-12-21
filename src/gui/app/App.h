#pragma once
#include "StateMaschine.hpp"
#include "Definitions.h"
#include "Taskbar.h"
#include "Wallpaper.h"
#include "Sounds.h"
#include <SFML/Graphics.hpp>

struct Resources
{
	Taskbar taskbar;
	Wallpaper wallpaper;
	Sounds sounds;
	sf::Font appFont;
	
	Resources(const sf::String& fontPath, sf::RenderWindow& window);
};

struct AppData
{
	sf::RenderWindow window;
	StateMaschine states;
	Resources resources{FONT_PATH, window};
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
