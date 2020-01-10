#pragma once
#include "SelbaWard/ConsoleScreen.h"
#include "../../Shell.hpp"
#include "../../SztosException.hpp"
#include <string>
#include <stdexcept>

inline void handleSystemOperations(Shell& shell, Cs& console, std::vector<std::string>& arguments)
{
	const auto cmd = arguments[0];

	for (int i = 0; i < arguments.size(); i++)
	{
		/* Przyklad korzystania z konsoli */
		if (i == 0)
		{
			console.log(arguments[0] + " ");
		}
		else
		{
			if (cmd == "Go" and arguments[i].empty()) {
				arguments[i] = "1";
			}
			console.print(arguments[i] + " ");
		}
	}
	console.println({});

	if (arguments.empty()) return;


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
		catch (SztosException &e)
		{
			console.println("Blad: " + std::string(e.what()));
			return;
		}
		catch (std::exception &e) {
			console.println("Nieznany blad: " + std::string(e.what()));
			return;
		}

		console.println("Process " + arguments[1] + " created.");
	}
	else if (cmd == "Go")
	{
		int numOfGo;

		try
		{
			numOfGo = std::stoi(arguments[1]);
		}
		catch (std::exception &e)
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


		for (int i = 0; i < numOfGo; ++i)
		{
			shell.getScheduler().schedulePcb();

			const auto pcb = shell.getScheduler().getRunningPcb();
			try
			{
				shell.getInterpreter().handleInsn(*pcb);
			}
			catch (SztosException &e) {
				console.println("Blad: " + std::string(e.what()));
				return;
			} catch (std::exception &e) {
				console.println("Nieznany blad: " + std::string(e.what()));
				return;
			}

			if (pcb->status == PCBStatus::Terminated
				|| pcb->status == PCBStatus::Error)
			{
				shell.getProcessManager().deleteProcessFromQueue(pcb->getPID());
				shell.getProcessManager().deleteProcessFromList(pcb->getPID());
				shell.getMemoryManager().deleteProgram(*pcb);
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
