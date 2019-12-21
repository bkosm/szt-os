#include "StateMaschine.hpp"

void StateMaschine::addState(std::unique_ptr<State> new_state, const bool is_replacing)
{
	isAdding_ = true;
	isReplacing_ = is_replacing;
	newState_ = std::move(new_state);
}

void StateMaschine::removeState()
{
	isRemoving_ = true;
}

void StateMaschine::processStateChanges()
{
	if (isRemoving_ && !states_.empty())
	{
		states_.pop();
		isRemoving_ = false;
	}
	if (isAdding_)
	{
		if (isReplacing_ && !states_.empty())
		{
			states_.pop();
		}
		
		states_.push(std::move(newState_));
		isAdding_ = false;
	}
}

std::unique_ptr<State>& StateMaschine::getActiveState()
{
	return states_.top();
}
