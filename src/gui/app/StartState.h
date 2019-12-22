#pragma once
#include "State.h"
#include "App.h"

class StartState final : public State
{
public:
	StartState(std::shared_ptr<AppData> data);

	void handleLeftClick_(const sf::Vector2f& mousePos, bool released = false);
	void handleRightClick_(const sf::Vector2f& mousePos, bool released = false);
	
	void handleInput() override;
	void update() override;
	void draw() override;
	
private:
	std::shared_ptr<AppData> data_;

	std::vector<Button> buttons_;
};
