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
		} catch (...) {/* TODO obsluga bledu utworzenia procesu */
			// wypisac blad
			return;
		}
		
		std::shared_ptr<PCB> pcb = shell.getProcessManager().getProcessFromList(arguments[1]);
		try {
			/* zaladowac program do pamieci */
		} catch (...) {/* TODO obsluga bledu zaladowania do pamieci */
			// wypisac blad
			return;
		}

		pcb->changeStatus(PCBStatus::Ready);
		shell.getProcessManager().addProcessToQueue(pcb);
		shell.getScheduler().onReadyPcb(pcb);

		console.println("Process " + arguments[1] + " created.");
	}
	else if (cmd == "Go")
	{
		/* ustalic ile razy */

		/* planista ustala proces ktory ma sie wykonywac */

		/* shell.getInterpreter().handleInsn(shell.getScheduler().getCurrentPCB());*/
		std::shared_ptr<PCB> pcb;

		if (pcb->status == PCBStatus::Terminated) {
			
		}
		/* sprawdzenie, czy proces sie zakonczyl */

		
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
