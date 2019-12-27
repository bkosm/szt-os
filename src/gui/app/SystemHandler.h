#pragma once
#include "SelbaWard/ConsoleScreen.h"
#include "../../../modules/Interpreter/Interpreter.hpp"


inline void handleSystemOperations(Cs& console, Shell& shell, std::vector<std::string>& arguments)
{
	for (int i = 0; i < arguments.size(); i++)
	{
		if (i == 0)
		{
			console.logln(arguments[0]);
		}
		else
		{
			console.println("\tArg " + std::to_string(i) + ": " + arguments[i]);
		}
	}
}
