#pragma once
#include "Widgets/Wallpaper.h"
#include "StateMaschine.h"
#include "Sounds.h"
#include "Definitions.h"
#include "../../Shell.hpp"
#include <SFML/Graphics.hpp>


struct Resources
{
	Wallpaper wallpaper;
	Sounds sounds;
	sf::Font appFont;
	sf::Texture runButtonTexture;

	Resources(const sf::String& fontPath, sf::RenderWindow& window);
};

struct AppData
{
	sf::RenderWindow window;
	Shell shell;
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
