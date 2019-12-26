#pragma once
#include "SelbaWard/ConsoleScreen.h"
#include "../../../modules/Interpreter/Interpreter.hpp"
#include <iostream>


inline void handleSystemOperations(Cs& console, Shell& shell, std::vector<std::string>& arguments)
{
	for (int i = 0; i < arguments.size(); i++)
	{
		if (i == 0)
		{
			console.log("Command: ");
			console << arguments[i] << Cs::CursorCommand::Newline;
		}
		else
		{
			console << Cs::CursorCommand::Tab;
			console.print("Arg " + std::to_string(i) + ": ");
			console << arguments[i] << Cs::CursorCommand::Newline;
		}
	}
}
