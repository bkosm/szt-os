#include "ShutdownState.h"

ShutdownState::ShutdownState(std::shared_ptr<AppData> data) :
	data_(std::move(data)),
	firstDraw_(true)
{
	fade_ = sf::RectangleShape(sf::Vector2f(RESOLUTION.width, RESOLUTION.height));
	fade_.setFillColor(sf::Color(0, 0, 0, 50));

	data_->resources.sounds.shutdown.play();
}

void ShutdownState::update()
{
	sf::Event event{};

	while (data_->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			{
				data_->resources.sounds.error.play();
				break;
			}
		default: break;
		}
	}

	if (data_->resources.sounds.shutdown.getStatus() == sf::Music::Stopped) data_->window.close();
}

void ShutdownState::draw()
{
	if (firstDraw_)
	{
		data_->window.clear(sf::Color::Black);
		data_->resources.wallpaper.draw();

		firstDraw_ = false;
	}

	data_->window.draw(fade_);
}
