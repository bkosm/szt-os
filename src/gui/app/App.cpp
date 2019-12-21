#include "App.h"
#include "StartState.h"

Resources::Resources(const sf::String& fontPath)
{
	appFont.loadFromFile(fontPath);
}

App::App(const sf::VideoMode& videoMode, const sf::String& title)
{
	data_->window.create(videoMode, title, sf::Style::Close);
	data_->window.setFramerateLimit(60);

	data_->states.addState(std::unique_ptr<State>(std::make_unique<StartState>(data_)), false);

	run_();
}

void App::run_() const
{
	while (data_->window.isOpen())
	{
		data_->states.processStateChanges();

		data_->states.getActiveState()->handleInput();
		data_->states.getActiveState()->update();

		data_->states.getActiveState()->draw();
		data_->window.display();
	}
}
