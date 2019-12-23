#pragma once
#include "App.h"
#include "State.h"
#include <memory>

class ShutdownState final : public State
{
public:
	ShutdownState(std::shared_ptr<AppData> data);

	void update() override;
	void draw() override;

private:
	std::shared_ptr<AppData> data_;

	sf::RectangleShape fade_;
	bool firstDraw_{true};
};
