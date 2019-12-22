#include "App.h"
#include "StartState.h"
#include <Windows.h>


Resources::Resources(const sf::String& fontPath, sf::RenderWindow& window) :
	taskbar(window),
	wallpaper(window)
{
	appFont.loadFromFile(fontPath);
	taskbar.setFont(appFont);
}
void AppData::systemShutdown()
{
	resources.sounds.shutdown.play();
	window.clear(sf::Color::Blue);
	
	Sleep(SHUTDOWN_DURATION);
	window.close();
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
	data_->resources.sounds.boot.play();
	
	while (data_->window.isOpen())
	{
		data_->states.processStateChanges();

		data_->states.getActiveState()->handleInput();
		data_->states.getActiveState()->update();

		/* Drawing section */
		data_->window.clear();

		data_->resources.wallpaper.draw();
		data_->states.getActiveState()->draw();
		data_->resources.taskbar.drawTaskbar();

		data_->window.display();
	}
}
