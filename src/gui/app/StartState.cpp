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
	/* Przycisk start */
	if (taskbar_.containsStartButton(mousePos))
	{
		taskbar_.toggleMenuDraw();
	}
	else if (taskbar_.containsShutdownButton(mousePos))
	{
		data_->states.addState(std::make_unique<ShutdownState>(data_), true);
	}
	else if (taskbar_.containsMenuButton(mousePos))
	{
		taskbar_.toggleMenuDraw();

		if (drawButtons_) {
			drawButtons_ = false;
			for (auto& button : buttons_) {
				button.draw = false;
				for (auto& command : button.commands) {
					command.draw = false;
				}
			}
		}
		else {
			drawButtons_ = true;
			for (auto& button : buttons_) {
				button.draw = true;
			}
		}
		return;
	}
	/* Ikona terminala */
	else if (taskbar_.containsTermButton(mousePos))
	{
		drawConsole_ = taskbar_.toggleTermButton();
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
					std::vector<std::string> input = { command.getInstruction() };

					for (auto& field : command.inputFields)
					{
						input.push_back(field.getInput());
					}
					/* handluj z konsekwencjami */
					try {
						handleSystemOperations(data_->shell, consoleScreen_, input);
					}
					catch (std::exception & e) {
						consoleScreen_.println(BIKE);
						consoleScreen_.println("Congratulations, we haven't expected that!");
					}

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
	consoleScreen_.setTextureTileSize({ 8, 12 });
	consoleScreen_.setNumberOfTextureTilesPerRow(16);
	consoleScreen_.setSize(consoleScreen_.getPerfectSize());

	consoleScreen_.setPosition(float(RESOLUTION.width / 2),
		float(RESOLUTION.height / 2 - consoleScreen_.getGlobalBounds().height / 2));
}

void StartState::configureButtons_()
{
	buttons_ = {
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 10.0,
			   START_STATE_BUTTON_SIZE, "Processor Status"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 22.0,
			   START_STATE_BUTTON_SIZE, "Process Manager 1"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 34.0,
			   START_STATE_BUTTON_SIZE, "Process Manager 2"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 46.0,
			   START_STATE_BUTTON_SIZE, "Memory Manager"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 58.0,
			   START_STATE_BUTTON_SIZE, "File Manager 1"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 70.0,
			   START_STATE_BUTTON_SIZE, "File Manager 2"),
		Button(float(RESOLUTION.width) / 8.0, float(RESOLUTION.height) / 100.0 * 82.0,
			   START_STATE_BUTTON_SIZE, "File Manager 3")
	};

	for (auto& button : buttons_) button.setLabelFont(data_->resources.appFont);

	/* Processor Status command buttons */
	/* GO */
	buttons_[0].commands.emplace_back("Go", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[0].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[0].commands[0].inputFields[0].setLabelAndFont("Number of runs:", data_->resources.appFont);
	/* SHOW PRIORITY */
	buttons_[0].commands.emplace_back("Show Priority", 0, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[0].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 20 / 100));
	/* SHOW LAST INSTRUCTION */
	buttons_[0].commands.emplace_back("Show Last Instruction", 0, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[0].commands[2].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 39 / 100));
	/* PARSE PROGRAM LABELS */
	buttons_[0].commands.emplace_back("Parse Program Labels", 2, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[0].commands[3].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 55 / 100));
	buttons_[0].commands[3].inputFields[0].setLabelAndFont("Input file name:", data_->resources.appFont);
	buttons_[0].commands[3].inputFields[1].setLabelAndFont("Output file name:", data_->resources.appFont);
	/* Process Manager 1 command buttons */
	/* CREATE PROCESS */
	buttons_[1].commands.emplace_back("Create Process", 2, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[1].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[1].commands[0].inputFields[0].setLabelAndFont("Process name:", data_->resources.appFont);
	buttons_[1].commands[0].inputFields[1].setLabelAndFont("Filepath of the program:", data_->resources.appFont);
	/* KILL PROCESS */
	buttons_[1].commands.emplace_back("Kill Process", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[1].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 28 / 100));
	buttons_[1].commands[1].inputFields[0].setLabelAndFont("Process ID:", data_->resources.appFont);
	/* CHANGE STATUS */
	buttons_[1].commands.emplace_back("Change Status", 2, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[1].commands[2].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 45 / 100));
	buttons_[1].commands[2].inputFields[0].setLabelAndFont("Process ID:", data_->resources.appFont);
	buttons_[1].commands[2].inputFields[1].setLabelAndFont("Status:", data_->resources.appFont);
	/* SHOW PROCESSES */
	buttons_[1].commands.emplace_back("Show Processes", 0, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[1].commands[3].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 71 / 100));
	/* Process Manager 2 command buttons */
	/* Memory Manager command buttons */
	/* SHOW MEMORY */
	buttons_[3].commands.emplace_back("Show Memory", 0, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[3].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	/* SHOW FRAME */
	buttons_[3].commands.emplace_back("Show Frame", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[3].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 22 / 100));
	buttons_[3].commands[1].inputFields[0].setLabelAndFont("Frame number:", data_->resources.appFont);
	/* SHOW PROCESS PAGES */
	buttons_[3].commands.emplace_back("Show Process Pages", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[3].commands[2].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 39 / 100));
	buttons_[3].commands[2].inputFields[0].setLabelAndFont("Process ID:", data_->resources.appFont);
	/* File Manager 1 command buttons */
	/* CREATE FILE */
	buttons_[4].commands.emplace_back("Create File", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[4].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[4].commands[0].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	/* SHOW FILE INFO */
	buttons_[4].commands.emplace_back("Show File Info", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[4].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 20 / 100));
	buttons_[4].commands[1].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	/* WRITE TO FILE */
	buttons_[4].commands.emplace_back("Write To File", 2, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[4].commands[2].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 37 / 100));
	buttons_[4].commands[2].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	buttons_[4].commands[2].inputFields[1].setLabelAndFont("Data:", data_->resources.appFont);
	/* SHOW DISK */
	buttons_[4].commands.emplace_back("Show Disk", 0, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[4].commands[3].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 62 / 100));
	/* SHOW File */
	buttons_[4].commands.emplace_back("Show File", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[4].commands[4].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 78 / 100));
	buttons_[4].commands[4].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	/* File Manager 2 command buttons */
	/* Delete File */
	buttons_[5].commands.emplace_back("Delete File", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[5].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[5].commands[0].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	/* Rename File */
	buttons_[5].commands.emplace_back("Rename File", 2, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[5].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 20 / 100));
	buttons_[5].commands[1].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	buttons_[5].commands[1].inputFields[1].setLabelAndFont("New file name:", data_->resources.appFont);
	/* List Files */
	buttons_[5].commands.emplace_back("List Files", 0, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[5].commands[2].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 45 / 100));
	/* Show Block */
	buttons_[5].commands.emplace_back("Show Block", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[5].commands[3].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 61 / 100));
	buttons_[5].commands[3].inputFields[0].setLabelAndFont("Block number:", data_->resources.appFont);
	/* File Manager 3 command buttons */
	/* Open File */
	buttons_[6].commands.emplace_back("Open File", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[6].commands[0].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 3 / 100));
	buttons_[6].commands[0].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	/* Close File */
	buttons_[6].commands.emplace_back("Close File", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[6].commands[1].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 21 / 100));
	buttons_[6].commands[1].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
	/* Show Lock Queue */
	buttons_[6].commands.emplace_back("Show Lock Queue", 1, data_->resources.appFont, data_->resources.runButtonTexture);
	buttons_[6].commands[2].setPosition(sf::Vector2f(RESOLUTION.width * 24 / 100, RESOLUTION.height * 38 / 100));
	buttons_[6].commands[2].inputFields[0].setLabelAndFont("File name:", data_->resources.appFont);
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
