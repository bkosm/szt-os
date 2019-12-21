#pragma once
#include "State.h"
#include "App.h"
#include "Button.h"

class StartState final : public State
{
public:
	StartState(std::shared_ptr<AppData> data);

	void handleInput() override;
	void update() override;
	void draw() override;
	
private:
	std::shared_ptr<AppData> data_;

	std::vector<Button> buttons_;
};
