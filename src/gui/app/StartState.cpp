#include "StartState.h"
#include <SFML/Graphics.hpp>
#include <utility>
#include <Windows.h>

StartState::StartState(std::shared_ptr<AppData> data) :
	data_(std::move(data))
{
	buttons_ = {
		Button(float(RESOLUTION.width) / 12.0 * 2, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "CP"),
		Button(float(RESOLUTION.width) / 12.0 * 4, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "DP"),
		Button(float(RESOLUTION.width) / 12.0 * 6, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "CF"),
		Button(float(RESOLUTION.width) / 12.0 * 8, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "DF"),
		Button(float(RESOLUTION.width) / 12.0 * 10, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "SP"),
	};

	for (auto& button : buttons_) button.setLabelFont(data_->resources.appFont);
}

void StartState::handleInput()
{
	sf::Event event{};

	while (data_->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			{
				data_->resources.sounds.shutdown.play();
				Sleep(3000);
				
				data_->window.close();
				break;
			}
		default:
			{
				break;
			}
		}
	}
}

void StartState::update()
{
}

void StartState::draw()
{
	for (const auto& button : buttons_) button.drawTo(data_->window);
}
