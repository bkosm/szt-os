#pragma once
#include "App.h"
#include "State.h"
#include "Widgets/Button.h"
#include "SelbaWard/ConsoleScreen.h"
#include "Widgets/Taskbar.h"

class StartState final : public State
{
public:
	StartState(std::shared_ptr<AppData> data);

	void update() override;
	void draw() override;

private:
	std::shared_ptr<AppData> data_;

	Cs consoleScreen_;
	bool drawConsole_{ false };
	bool drawButtons_{ false };

	Taskbar taskbar_;
	std::vector<Button> buttons_;

	void handleLeftClick_(const sf::Vector2f& mousePos, bool released = false);
	void handleRightClick_(const sf::Vector2f& mousePos, bool released = false);

	void consoleSetup_();
	void configureButtons_();
};
