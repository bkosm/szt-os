#pragma once
#include <SFML/Graphics.hpp>

class Wallpaper
{
public:
	Wallpaper(sf::RenderWindow& window);
	~Wallpaper() = default;

	void draw() const;

private:
	sf::RenderWindow& window_;
	
	sf::Texture textures_[2];
	sf::Sprite sprites_[2];
};

