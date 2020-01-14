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
		catch (SztosException & e)
		{
			console.println("Blad: " + std::string(e.what()));
			return;
		}
		catch (std::exception & e) {
			console.println("Nieznany blad: " + std::string(e.what()));
			return;
		}

		console.println("Process " + arguments[1] + " was created.");
	}
	else if (cmd == "Go")
	{
		int numOfGo;

		try
		{
			numOfGo = std::stoi(arguments[1]);
		}
		catch (std::exception & e)
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
			catch (SztosException & e) {
				console.println("Blad: " + std::string(e.what()));
				return;
			}
			catch (std::exception & e) {
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
		shell.getProcessManager().deleteProcessFromQueue(std::stoi(arguments[1]));
		shell.getProcessManager().deleteProcessFromList(std::stoi(arguments[1]));
		// TODO free RAM
		console.println("Killed process.");
	}
	else if (cmd == "Change Status")
	{
		shell.getProcessManager().changeStatusChosenProcess(std::stoi(arguments[1]), shell.getProcessManager().convertStringToPCBStatus(arguments[2]));
		console.println("Changed status for process.");
	}
	else if (cmd == "Show Memory")
	{
		console.println(shell.getMemoryManager().showMemory());
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
		console.println(shell.getProcessManager().showPriority());
	}
	else if (cmd == "Show Processes")
	{
		console.println(shell.getProcessManager().showProcessList());
	}
	else if (cmd == "Create File")
	{
		shell.getFileManager().createFile(arguments[1]);
	}
	else if (cmd == "Show File Info")
	{
		const auto info = shell.getFileManager().displayFileInfo(arguments[1]);
		console.println(info);
	}
	else if (cmd == "Show Disk")
	{
		const auto disk = shell.getFileManager().displayDiskContentChar();
		console.println(disk);
	}
	else if (cmd == "Write To File")
	{
		if (arguments[1].empty() or arguments[2].empty())
		{
			console.println("Nie podano argumentow.");
			return;
		}

		shell.getFileManager().openFile(arguments[1]);
		shell.getFileManager().writeToFile(arguments[1], arguments[2]);
		shell.getFileManager().closeFile(arguments[1]);
	}
	else if (cmd == "Show File")
	{
		shell.getFileManager().openFile(arguments[1]);
		console.println(shell.getFileManager().readFileAll(arguments[1]));
		shell.getFileManager().closeFile(arguments[1]);
	}
	else if (cmd == "Delete File")
	{
		if (arguments[1].empty())
		{
			console.println("Nie podano argumentu.");
			return;
		}

		shell.getFileManager().deleteFile(arguments[1]);
	}
}
