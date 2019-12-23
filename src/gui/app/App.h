#pragma once
#include "Widgets/Wallpaper.h"
#include "StateMaschine.h"
#include "Sounds.h"
#include "Definitions.h"

struct Resources
{
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
