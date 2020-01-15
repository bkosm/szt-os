#pragma once
#include "Definitions.h"
#include "SelbaWard/ConsoleScreen.h"
#include "../../Shell.hpp"
#include "../../SztosException.hpp"
#include "../../modules/Interpreter/LabelParser.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>
#include <cctype>



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
	for (auto arg : arguments) {
		if (arg.empty()) {
			console.println("Invalid arguments.");
			return;
		}
	}

	if (cmd == "Create Process")
	{
		/* if (sprawdzenie poprawnosci argumentow) */
		if (arguments[1].empty() or arguments[2].empty())
		{
			console.println("Arguments cannot be empty.");
			return;
		}

		try
		{
			shell.getProcessManager().createProcess(arguments[1], arguments[2]);
			if (arguments[1] == "rowerek")
			{
				console.println(BIKE);
			}
		}
		catch (SztosException &e)
		{
			console.println("Error: " + std::string(e.what()));
			return;
		}
		catch (std::exception &e) {
			console.println("Unknown error: " + std::string(e.what()));
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
			console.println("Argument is not a number.");
			return;
		}
		if (numOfGo < 1)
		{
			console.println("The number of performances must be greater than zero.");
			return;
		}


		for (int i = 0; i < numOfGo; ++i)
		{
			std::vector<std::shared_ptr<PCB>> terminatedPcbs;
			for (auto pcbPtr : shell.getProcessManager().getProcessList())
			{
				if (pcbPtr->status == PCBStatus::Terminated)
				{
					terminatedPcbs.push_back(pcbPtr);
				}
			}
			
			for (auto pcbPtr : terminatedPcbs) {
				try
				{
					shell.getProcessManager().deleteProcessFromList(pcbPtr->getPID());
				}
				catch (SztosException & e)
				{
					console.println("Encountered error that should not happen ??? (pre-clear terminated processes) (Error: " + std::string(e.what()) + ").");
				}

				shell.getMemoryManager().deleteProgram(*pcbPtr);
			}

			const auto pcb = shell.getScheduler().getRunningPcb();
			try {
				shell.getInterpreter().handleInsn(pcb);
			}
			catch (SztosException & e) {
				console.println("Error: " + std::string(e.what()));
				return;
			}
			catch (std::exception & e) {
				console.println("Unknown error: " + std::string(e.what()));
				return;
			}

			if (pcb->status == PCBStatus::Terminated)
			{
				try {
					shell.getProcessManager().deleteProcessFromQueue(pcb->getPID());
					
					console.println("Process was terminated at " + std::to_string(i + 1)
						+ " iteration. You can check process memory before next GO command.");
				} catch (SztosException &e)
				{
					console.println("Encountered error that should not happen ??? (on-terminate) (Error: " + std::string(e.what()) + ").");
				}
				
				break;
			}
			else if (pcb->status == PCBStatus::Error)
			{
				try {
					shell.getProcessManager().deleteProcessFromQueue(pcb->getPID());

					console.println("Process encountered error at " + std::to_string(i + 1)
						+ " iteration. You can check process memory. Process with error have to be removed manually.");
				}
				catch (SztosException & e)
				{
					console.println("Encountered error that should not happen ??? (on-error) (Error: " + std::string(e.what()) + ").");
				}

				break;
			}
		}
	}
	else if (cmd == "Kill Process")
	{
		std::shared_ptr<PCB> pcb = nullptr;
		try
		{
			pcb = shell.getProcessManager().getProcessFromList(std::stoi(arguments[1]));
		}
		catch (SztosException &e)
		{
			console.println("Error: " + std::string(e.what()));
			return;
		}
		catch (std::exception & e) {
			console.println("Unknown error: " + std::string(e.what()));
			return;
		}


		if (pcb->getStatus() == PCBStatus::Running)
		{
			console.println("The Running status process cannot be deleted.");
			return;
		}

		try
		{
			shell.getProcessManager().deleteProcessFromList(pcb->getPID());
			shell.getMemoryManager().deleteProgram(*pcb);
			console.println("Killed process.");
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}
		catch (std::exception & e) {
			console.println("Unknown error: " + std::string(e.what()));
			return;
		}
	}
	else if (cmd == "Change Status")
	{
		std::shared_ptr<PCB> pcb = nullptr;
		try
		{
			pcb = shell.getProcessManager().getProcessFromList(std::stoi(arguments[1]));
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
			return;
		}
		catch (std::exception & e) {
			console.println("Unknown error: " + std::string(e.what()));
			return;
		}


		if (pcb->getPID() == 0)
		{
			console.println("You cannot touch the DUMMY process.");
			return;
		}
		if (shell.getProcessManager().getProcessList().size() <= std::stoi(arguments[1]))
		{
			console.println("There is no process with the given ID.");
			return;
		}

		try
		{
			shell.getProcessManager().changeStatusChosenProcess(std::stoi(arguments[1]), shell.getProcessManager().convertStringToPCBStatus(arguments[2]));
			console.println("Changed status for process.");
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
			return;
		}
		catch (std::exception & e) {
			console.println("Unknown error: " + std::string(e.what()));
			return;
		}
	}
	else if (cmd == "Show Memory")
	{
		console.println(shell.getMemoryManager().showMemory());
	}
	else if (cmd == "Show Priority")
	{
		console.println(shell.getProcessManager().showPriority());
	}
	else if (cmd == "Show Processes")
	{
		console.println(shell.getProcessManager().showProcessList());
	}
	else if (cmd == "Create File") //
	{
		if (arguments[1].empty())
		{
			console.println("No argument provided.");
			return;
		}

		bool exist = false;
		for (auto file : shell.getFileManager().getFiles()) {

			if (file.name == arguments[1]) {
				exist = true;
			}
			if (exist == true) {
				console.println("A file with the given name already exists.");
				return;
			}
		}

		shell.getFileManager().createFile(arguments[1]);
		console.println("File " + arguments[1] + " was created.");
	}
	else if (cmd == "Show File Info")
	{
		try
		{
			const auto info = shell.getFileManager().displayFileInfo(arguments[1]);
			console.println(info);
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}
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
			console.println("No argument provided.");
			return;
		}
		if (shell.getFileManager().getFiles().empty())
		{
			console.println("File list is empty.");
			return;
		}

		try
		{
			const auto code = shell.getFileManager().openFile(arguments[1], nullptr);
			if (code == 0)
			{
				shell.getFileManager().writeToFile(arguments[1], arguments[2]);
				shell.getFileManager().closeFile(arguments[1], nullptr);
			}
			else
			{
				console.println("Could not open file.");
			}
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}
	}
	else if (cmd == "Show File")
	{
		try
		{
			const auto code = shell.getFileManager().openFile(arguments[1], nullptr);
			if (code == 0)
			{
				console.println(shell.getFileManager().readFileAll(arguments[1]));
				shell.getFileManager().closeFile(arguments[1], nullptr);
			}
			else
			{
				console.println("Could not open file.");
			}
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}
	}
	else if (cmd == "Delete File")
	{
		if (arguments[1].empty())
		{
			console.println("No argument provided.");
			return;
		}

		try
		{
			const auto code = shell.getFileManager().openFile(arguments[1], nullptr);
			if (code == 0)
			{
				shell.getFileManager().deleteFile(arguments[1]);
			}
			else
			{
				console.println("No file access.");
			}
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}
	}
	else if (cmd == "List Files")
	{
		console.println(shell.getFileManager().fileList());
	}
	else if (cmd == "Open File")
	{
		try
		{
			const auto code = shell.getFileManager().openFile(arguments[1], nullptr);

			if (code == 0)
			{
				console.println("Open file.");
			}
			else
			{
				console.println("Could not open file.");
			}
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}
	}
	else if (cmd == "Close File")
	{
		try
		{
			const auto code = shell.getFileManager().closeFile(arguments[1], nullptr);
		}
		catch (SztosException & e)
		{
			console.println("Error: " + std::string(e.what()));
		}

	}
	else if (cmd == "Show Block")
	{
		try
		{
			if (std::stoi(arguments[1]) < 0 || std::stoi(arguments[1]) >= 64)
			{
				console.println("Memory range exceeded.");
				return;
			}
			console.println(shell.getFileManager().showBlock(std::stoi(arguments[1])));

		}
		catch (std::exception & e)
		{
			console.println("Error: " + std::string(e.what()) + ".");
		}
	}
	else if (cmd == "Show Lock Queue") // DO ZROBIENIA 
	{

		console.println(shell.getFileManager().fileLockQueue(arguments[1]));
	}
	else if (cmd == "Show Frame") /* DODANE - EDYCJA */
	{
		int frameNumber;

		try
		{
			frameNumber = std::stoi(arguments[1]);
		}
		catch (std::exception & e)
		{
			console.println("Error: " + std::string(e.what()) + ".");
			return;
		}

		if (arguments[1].empty())
		{
			console.println("No argument provided.");
			return;
		}
		if (frameNumber < 0 || frameNumber > 31)
		{
			console.println("The argument provided is out of range.");
			return;
		}

		console.println(shell.getMemoryManager().showFrame(frameNumber));
	}
	else if (cmd == "Show Last Instruction") {
		console.println("Last insn: " + shell.getInterpreter().getLastInsn());
	}
	else if (cmd == "Parse Program Labels") {
		try
		{
			parseCode(arguments[1], arguments[2]);
			console.println("LabelParser has been executed.");
		} catch (SztosException &e)
		{
			console.println("Error: " + std::string(e.what()));
			return;
		}
		catch (std::exception & e) {
			console.println("Unknown error: " + std::string(e.what()));
			return;
		}
		
	}
	else if (cmd == "Show Process Pages") {
		try
		{
			console.println(shell.getProcessManager().showProcessPages(std::stoi(arguments[1])));
		}
		catch (SztosException& e)
		{
			console.println("Error: " + std::string(e.what()));
		}
		catch (std::exception& e)
		{
			console.println("Error: " + std::string(e.what()));
		}
	}
}
