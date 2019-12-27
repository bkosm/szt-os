#pragma once
#include "SelbaWard/ConsoleScreen.h"
#include "../../Shell.hpp"
#include <iostream>


inline void handleSystemOperations(Cs& console, Shell& shell, std::vector<std::string>& arguments)
{
	const selbaward::ConsoleScreen::CursorCommand NewLine = Cs::CursorCommand::Newline;
	const selbaward::ConsoleScreen::CursorCommand Tab = Cs::CursorCommand::Tab;
	for (int i = 0; i < arguments.size(); i++)
	{
		if (i == 0)
		{
			console.log("Command: ");
			console << arguments[i] << NewLine;
		}
		else
		{
			console << Tab;
			console.print("Arg " + std::to_string(i) + ": ");
			console << arguments[i] << NewLine;
		}
	}

	if (arguments[0] == "Create Process")
	{
		console << "Creating process" << NewLine;
	}
	else if (arguments[0] == "GO") {
		// scheduler
	}
	else if(arguments[0] == "Change Status")
	{
		
	}
	else if(arguments[0] == "Show Pages")
	{
		
	}
	else if(arguments[0] == "Show Lock")
	{
		
	}
	else if(arguments[0] == "Long File")
	{
		
	}
	else if(arguments[0] == "Show i-note")
	{
		
	}
	else if(arguments[0] == "Show Priority")
	{
		
	}
	
}
