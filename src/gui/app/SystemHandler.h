#pragma once
#include "SelbaWard/ConsoleScreen.h"
#include "../../Shell.hpp"

inline void handleSystemOperations(Shell& shell, Cs& console, std::vector<std::string>& arguments)
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

	if (arguments.empty()) return;

	std::string cmd = arguments[0];

	if (cmd == "Create Process")
	{
		/* if (sprawdzenie poprawnosci argumentow) */

		try {
			shell.getProcessManager().createProcess(arguments[1], arguments[2]);
		} catch (std::invalid_argument &e) {
			/* wypisac blad: nie ma takiego programu */
			return;
		} catch (std::overflow_error &e) {
			/* wypisac blad: nie ma miejsca w pamieci */
			return;
		}

		console.println("Process " + arguments[1] + " created.");
	}
	else if (cmd == "Go")
	{
		/* ustalic ile razy */

		shell.getScheduler().schedulePcb();

		auto pcb = shell.getScheduler().getRunningProcess();
		try {
			shell.getInterpreter().handleInsn(*pcb);
		} catch (std::exception &e) {
			/* TODO: obsluga bledow (jak moduly beda skonczone) */
		}
		

		if (pcb->status == PCBStatus::Terminated) {
			shell.getProcessManager().deleteProcessFromQueue(pcb->getPID());
			/* TODO: usunac proces z listy procesow i kolejki */
		}
		
	}
	else if(cmd == "Kill Process")
	{
	}
	else if (cmd == "Change Status")
	{
	}
	else if (cmd == "Show Pages")
	{
	}
	else if (cmd == "Show Lock")
	{
	}
	else if (cmd == "Long File")
	{
	}
	else if (cmd == "Show i-node") //chyba nie mamy tego xd
	{
	}
	else if (cmd == "Show Priority")
	{
	}
}
