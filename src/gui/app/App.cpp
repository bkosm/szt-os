#include "App.h"
#include "StartState.h"
#include "Definitions.h"

Resources::Resources(const sf::String& fontPath, sf::RenderWindow& window) :
	wallpaper(window)
{
	appFont.loadFromFile(fontPath);
	runButtonTexture.loadFromFile(PLAY_BUTTON_PATH);
}

App::App(const sf::VideoMode& videoMode, const sf::String& title)
{
	auto dummy = data_->shell.getProcessManager().createDummyProcess();
	data_->shell.getScheduler().setRunningPcb(dummy);

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
