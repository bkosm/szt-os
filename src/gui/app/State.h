#pragma once

class State
{
public:
	virtual ~State() = default;

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
};