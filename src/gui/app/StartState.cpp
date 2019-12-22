#include "StartState.h"
#include <SFML/Graphics.hpp>
#include <utility>

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

void StartState::handleLeftClick_(const sf::Vector2f& mousePos, bool released)
{
	if (released)
	{
		for (auto& button : buttons_)
		{
			button.markAsReleased();
		}
	}
	else
	{
		/* Przyciski pulpitu */
		for (auto& button : buttons_)
		{
			if (button.getGlobalBounds().contains(mousePos))
			{
				button.markAsClicked();
			}
		}

		/* Przycisk start */
		if (data_->resources.taskbar.containsStartButton(mousePos))
		{
			data_->resources.taskbar.toggleMenuDraw();
		}
		else if (data_->resources.taskbar.containsShutdownButton(mousePos))
		{
			data_->systemShutdown();
		}
	}
}

void StartState::handleRightClick_(const sf::Vector2f& mousePos, bool released)
{
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
				data_->systemShutdown();
				break;
			}

		case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					const auto position = sf::Mouse::getPosition(data_->window);
					handleLeftClick_(sf::Vector2f(position.x, position.y));
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					const auto position = sf::Mouse::getPosition(data_->window);
					handleRightClick_(sf::Vector2f(position.x, position.y));
				}
				break;
			}
		case sf::Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					const auto position = sf::Mouse::getPosition(data_->window);
					handleLeftClick_(sf::Vector2f(position.x, position.y), true);
				}
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					const auto position = sf::Mouse::getPosition(data_->window);
					handleRightClick_(sf::Vector2f(position.x, position.y), true);
				}
				break;
			}
		default: break;
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
