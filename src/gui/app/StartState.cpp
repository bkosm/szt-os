#include "StartState.h"
#include "ShutdownState.h"
#include <SFML/Graphics.hpp>
#include <utility>

StartState::StartState(std::shared_ptr<AppData> data) :
	data_(std::move(data)),
	consoleScreen_(CONSOLE_FONT_PATH)
{
	taskbar_.setFont(data_->resources.appFont);
	consoleSetup_();

	buttons_ = {
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "Processor Status"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0 * 2.0,
		       START_STATE_BUTTON_SIZE, "Process Management"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0 * 3.0,
		       START_STATE_BUTTON_SIZE, "Memory Management"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0 * 4.0,
		       START_STATE_BUTTON_SIZE, "File Management"),
	};

	for (auto& button : buttons_) button.setLabelFont(data_->resources.appFont);
}

void StartState::handleLeftClick_(const sf::Vector2f& mousePos, bool released)
{
	consoleScreen_.println("lewa siema!");

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
		if (taskbar_.containsStartButton(mousePos))
		{
			taskbar_.toggleMenuDraw();
		}
		else if (taskbar_.containsShutdownButton(mousePos))
		{
			data_->states.addState(std::make_unique<ShutdownState>(data_), true);
		}

		/* Ikona terminala */
		if (taskbar_.containsTermButton(mousePos))
		{
			taskbar_.toggleTermButton();
		}
	}
}

void StartState::handleRightClick_(const sf::Vector2f& mousePos, bool released)
{
	consoleScreen_.logln("no siema!");
	taskbar_.dontDrawMenu();
}

void StartState::consoleSetup_()
{
	consoleScreen_.setTextureTileSize({8, 12});
	consoleScreen_.setNumberOfTextureTilesPerRow(16);
	consoleScreen_.setSize(consoleScreen_.getPerfectSize());

	consoleScreen_.setPosition(float(RESOLUTION.width / 2),
	                           float(RESOLUTION.height / 2 - consoleScreen_.getGlobalBounds().height / 2));
}

void StartState::update()
{
	sf::Event event{};

	while (data_->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			{
				data_->states.addState(std::make_unique<ShutdownState>(data_), true);
				return;
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

void StartState::draw()
{
	data_->window.clear();

	data_->resources.wallpaper.draw();

	for (const auto& button : buttons_) button.drawTo(data_->window);
	data_->window.draw(consoleScreen_);

	taskbar_.drawTo(data_->window);
}