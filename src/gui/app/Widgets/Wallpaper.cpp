#include "../Definitions.h"
#include "Wallpaper.h"

Wallpaper::Wallpaper(sf::RenderWindow& window) :
	window_(window)
{
	textures_[0].loadFromFile(SZTOS_WALLPAPER_PATH_1);
	textures_[1].loadFromFile(SZTOS_WALLPAPER_PATH_2);

	sprites_[0].setTexture(textures_[0]);
	sprites_[1].setTexture(textures_[1]);

	const auto wallGB = sprites_[0].getGlobalBounds();

	sprites_[0].setPosition(0, 0);
	sprites_[1].setPosition(wallGB.width, 0);
}

void Wallpaper::draw() const
{
	window_.draw(sprites_[0]);
	window_.draw(sprites_[1]);
}
