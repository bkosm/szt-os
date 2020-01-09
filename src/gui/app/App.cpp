#include "App.h"
#include "StartState.h"
#include "Definitions.h"
#include <iostream>

Resources::Resources(const sf::String& fontPath, sf::RenderWindow& window) :
	wallpaper(window)
{
	appFont.loadFromFile(fontPath);
	runButtonTexture.loadFromFile(PLAY_BUTTON_PATH);
}

App::App(const sf::VideoMode& videoMode, const sf::String& title)
{
	try
	{
		auto dummy = data_->shell.getProcessManager().createDummyProcess();
		data_->shell.getScheduler().setRunningPcb(dummy);
	}
	catch (std::invalid_argument & e)
	{
		std::cout << "Nie udalo sie zaladowac programu dummy.txt." << std::endl;
		return;
	}
	catch (std::overflow_error & e)
	{
		std::cout << "Nie ma miejsca w pamieci na program dummy.txt." << std::endl;
		return;
	}

	data_->window.create(videoMode, title, sf::Style::Close);
	data_->window.setFramerateLimit(60);

	data_->states.addState(std::unique_ptr<State>(std::make_unique<StartState>(data_)), false);

	run_();
}

void App::run_() const
{
	data_->resources.sounds.boot.play();
	
	while (data_->window.isOpen())
	{
		data_->states.processStateChanges();
		data_->states.getActiveState()->update();

		data_->states.getActiveState()->draw();

		data_->window.display();
	}
}
