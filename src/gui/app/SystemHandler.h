#pragma once
#include "SelbaWard/ConsoleScreen.h"

class Shell;

inline void handleSystemOperations(Cs& console, std::vector<std::string>& arguments)
{
	for (int i = 0; i < arguments.size(); i++)
	{
		/* Przyklad korzystania z konsoli */
		if (i == 0)
		{
			console.logln("Command: " + arguments[0]);
		}
		else
		{
			console.println("\tArg " + std::to_string(i) + ": " + arguments[i]);
		}
	}

	if (arguments[0] == "Create Process")
	{
	}
	else if (arguments[0] == "GO")
	{
	}
	else if (arguments[0] == "Change Status")
	{
	}
	else if (arguments[0] == "Show Pages")
	{
	}
	else if (arguments[0] == "Show Lock")
	{
	}
	else if (arguments[0] == "Long File")
	{
	}
	else if (arguments[0] == "Show i-node")
	{
	}
	else if (arguments[0] == "Show Priority")
	{
	}
}
