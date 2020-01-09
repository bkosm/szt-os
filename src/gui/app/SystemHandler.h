#pragma once
#include "SelbaWard/ConsoleScreen.h"
#include "../../Shell.hpp"
#include <string>

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

	const auto cmd = arguments[0];

	if (cmd == "Create Process")
	{
		/* if (sprawdzenie poprawnosci argumentow) */
		if (arguments[1].empty() or arguments[2].empty())
		{
			console.println("Argumenty nie moga byc puste.");
			return;
		}

		/* TODO: usunac nieprawidlowe znaki */

		try
		{
			shell.getProcessManager().createProcess(arguments[1], arguments[2]);
		}
		catch (std::invalid_argument & e)
		{
			console.println("Blad przy wczytywaniu programu: " + std::string(e.what()));
			return;
		}
		catch (std::overflow_error & e)
		{
			console.println("Blad przy wczytywaniu programu: " + std::string(e.what()));
			return;
		}

		console.println("Process " + arguments[1] + " created.");
	}
	else if (cmd == "Go")
	{
		if (arguments[1].empty())
		{
			console.println("Nie podano liczby wykonan.");
			return;
		}
		int numOfGo;
		try
		{
			numOfGo = std::stoi(arguments[1]);
		}
		catch (std::invalid_argument & e)
		{
			console.println("Argument nie jest liczba.");
			return;
		}
		/* robi flipa gdy nie ma gotowych procesow do odpalenia wiec nara */
		if (numOfGo < 1)
		{
			console.println("Ilosc wykonan musi byc wieksza od zera");
			return;
		}

		/* ilosc razy jest w arguments[1] */

		for (int i = 0; i < numOfGo; ++i)
		{
			shell.getScheduler().schedulePcb();

			const auto pcb = shell.getScheduler().getRunningPcb();
			try
			{
				shell.getInterpreter().handleInsn(*pcb);
			}
			catch (std::exception & e)
			{
				console.println("Napotkano blad: " + std::string(e.what()));
				continue;
			}

			if (pcb->status == PCBStatus::Terminated)
			{
				shell.getProcessManager().deleteProcessFromQueue(pcb->getPID());
				shell.getProcessManager().deleteProcessFromList(pcb->getPID());
			}
		}
	}
	else if (cmd == "Kill Process")
	{
	}
	else if (cmd == "Change Status")
	{
	}
	else if (cmd == "Show Memory")
	{
		console.println(shell.getMemoryManager().showMemory());
	}
	else if (cmd == "Show Lock")
	{
	}
	else if (cmd == "Long File")
	{
	}
	else if (cmd == "Show i-node") //chyba nie mamy tego xd
	{
		auto test = shell.getLockManager().createLock();

		console.println(test.getProcessQueueString());

		test.getProcessQueue().push_back(std::make_shared<PCB>(PCB("Siema", 123, PCBStatus::New)));
		test.getProcessQueue().push_back(std::make_shared<PCB>(PCB("Byku", 321, PCBStatus::Running)));

		console.println(test.getProcessQueueString());
	}
	else if (cmd == "Show Priority")
	{
	}
	else if (cmd == "Show Processes")
	{
		console.println(shell.getProcessManager().showProcessList());
	}
}
