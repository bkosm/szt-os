#include "SystemHandler.h"
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
	configureButtons_();
}

void StartState::handleLeftClick_(const sf::Vector2f& mousePos, bool released)
{
	if (released)
	{
		for (auto& button : buttons_)
		{
			button.markAsReleased();
			for (auto& command : button.commands)
			{
				command.markRunButtonReleased();
			}
		}

		return;
	}
	/* Przyciski pulpitu */
	for (auto& button : buttons_)
	{
		/* Glowny przycisk */
		if (button.getGlobalBounds().contains(mousePos))
		{
			for (auto& b : buttons_) b.hideCommands();

			button.markAsClicked();
			button.drawCommands();
		}
		else
		{
			/* Przyciski komend */
			for (auto& command : button.commands)
			{
				/* Przycisk startu komendy */
				if (command.containsRunButton(mousePos) and command.draw)
				{
					command.markRunButtonPressed();
					std::vector<std::string> input = {command.getInstruction()};

					for (auto& field : command.inputFields)
					{
						input.push_back(field.getInput());
					}

					handleSystemOperations(consoleScreen_, input);
				}
				else
				{
					/* Pola do wprowadzania argumentow */
					for (auto& field : command.inputFields)
					{
						if (field.contains(mousePos) and command.draw)
						{
							field.setFocused();
						}
					}
				}
			}
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
	else if (taskbar_.containsTermButton(mousePos))
	{
		drawConsole_ = taskbar_.toggleTermButton();
	}
}

void StartState::handleRightClick_(const sf::Vector2f& mousePos, bool released)
{
	taskbar_.dontDrawMenu();
	for (auto& button : buttons_) {
		button.hideCommands();
	}
}

void StartState::consoleSetup_()
{
	consoleScreen_.setTextureTileSize({8, 12});
	consoleScreen_.setNumberOfTextureTilesPerRow(16);
	consoleScreen_.setSize(consoleScreen_.getPerfectSize());

	consoleScreen_.setPosition(float(RESOLUTION.width / 2),
	                           float(RESOLUTION.height / 2 - consoleScreen_.getGlobalBounds().height / 2));
}

void StartState::configureButtons_()
{
	buttons_ = {
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0,
		       START_STATE_BUTTON_SIZE, "Processor Status"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0 * 2.0,
		       START_STATE_BUTTON_SIZE, "Process Manager"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0 * 3.0,
		       START_STATE_BUTTON_SIZE, "Memory Manager"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 5.0 * 4.0,
		       START_STATE_BUTTON_SIZE, "File Manager"),
	};

	for (auto& button : buttons_) button.setLabelFont(data_->resources.appFont);

	/* Processor Status command buttons */
	/* GO */
	buttons_[0].commands.emplace_back("Go", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[0].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[0].commands[0].inputFields[0].setLabelAndFont("Number of runs:", data_->resources.appFont);
	/* Process Manager command buttons */
	/* CP */
	buttons_[1].commands.emplace_back("Create Process", 2, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[1].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[1].commands[0].inputFields[0].setLabelAndFont("Process name:", data_->resources.appFont);
	buttons_[1].commands[0].inputFields[1].setLabelAndFont("File containing program:", data_->resources.appFont);
	/* KP */
	buttons_[1].commands.emplace_back("Kill Process", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[1].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 30 / 100));
	buttons_[1].commands[1].inputFields[0].setLabelAndFont("Process PID:", data_->resources.appFont);
	/* Memory Manager command buttons */

	/* File Manager command buttons */
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
					/* Unfocus all input fields before picking one */
					for (auto& button : buttons_)
					{
						for (auto& command : button.commands)
						{
							command.disableFocus();
						}
					}

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
		case sf::Event::TextEntered:
			{
				for (auto& button : buttons_)
				{
					for (auto& command : button.commands)
					{
						command.writeCommand(event.text.unicode);
					}
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

	for (const auto& button : buttons_)
	{
		button.drawTo(data_->window);
		for (const auto& command : button.commands)
		{
			command.drawTo(data_->window);
		}
	}

	if (drawConsole_) data_->window.draw(consoleScreen_);

	taskbar_.drawTo(data_->window);
}
