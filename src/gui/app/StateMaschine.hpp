#pragma once
#include "State.h"
#include <stack>
#include <memory>

class StateMaschine
{
public:
	StateMaschine() = default;
	~StateMaschine() = default;

	void addState(std::unique_ptr<State> new_state, bool is_replacing);
	void removeState();
	void processStateChanges();

	std::unique_ptr<State>& getActiveState();

private:
	std::unique_ptr<State> newState_;
	std::stack<std::unique_ptr<State>> states_;

	bool isRemoving_{}, isAdding_{}, isReplacing_{};
};
