#include "PCB.hpp"
#include "ProcessManager.hpp"
#include "../../Shell.hpp"
#include <random>
#include <algorithm>
#include <exception>
#include <iostream>
#include <utility>

using PCB_ptr = std::shared_ptr<PCB>;

ProcessManager::ProcessManager(Shell *shell) : shell(shell)
{
}

ProcessManager::~ProcessManager()
{
}

void ProcessManager::createProcess(std::string name, std::string fileName)
{
	auto pcb = std::make_shared<PCB>(std::move(name), getNextPID(), PCBStatus::New);
	processList.push_back(pcb);

	try {
		/* zaladowac program do pamieci */
	} catch (std::overflow_error &e) {
		deleteProcessFromList(pcb->getPID());
		throw e;
	} catch (std::invalid_argument &e) {
		deleteProcessFromList(pcb->getPID());
		throw e;
	}

	pcb->changeStatus(PCBStatus::Ready);
	shell->getProcessManager().addProcessToQueue(pcb);
	shell->getScheduler().onReadyPcb(pcb);
}

std::string ProcessManager::showChosenProcess(PCB_ptr process)
{
	std::string word;
	word = "| CHOSEN PROCESS |\n";
	word = word + "| " + process->processName + " | " + (char)process->processID + " | ";
	switch (process->status)
	{
	case PCBStatus::New:
		word = word + "NEW |\n";
		break;
	case PCBStatus::Ready:
		word = word + "READY |\n";
		break;
	case PCBStatus::Running:
		word = word + "RUNNING |\n";
		break;
	case PCBStatus::Waiting:
		word = word + "WAITING |\n";
		break;
	case PCBStatus::Terminated:
		word = word + "TERMINATED |\n"; //useless
		break;
	}
	return word;
}

std::string ProcessManager::showProcessList(std::vector<PCB_ptr> list)
{
	std::string word;
	word = "| PROCESS LIST |\n";
	for (auto const element : list)
	{
		word = word + "| " + element->processName + " | " + std::to_string(element->processID) + " | ";
		switch (element->status)
		{
		case PCBStatus::New:
			word = word + "NEW |\n";
			break;
		case PCBStatus::Ready:
			word = word + "READY |\n";
			break;
		case PCBStatus::Running:
			word = word + "RUNNING |\n";
			break;
		case PCBStatus::Waiting:
			word = word + "WAITING |\n";
			break;
		case PCBStatus::Terminated:
			word = word + "TERMINATED |\n"; //useless
			break;
		}
	}
	std::cout << word;
	return word;
}

std::string ProcessManager::showReadyQueue(std::vector<PCB_ptr> queue)
{
	std::string word;
	word = "| READY QUEUE |\n";
	for (auto const element : queue)
	{
		word = word + "| " + element->processName + " | " + std::to_string(element->processID) + " | ";
		switch (element->status)
		{
		case PCBStatus::New:
			word = word + "NEW |\n";
			break;
		case PCBStatus::Ready:
			word = word + "READY |\n";
			break;
		case PCBStatus::Running:
			word = word + "RUNNING |\n";
			break;
		case PCBStatus::Waiting:
			word = word + "WAITING |\n";
			break;
		case PCBStatus::Terminated:
			word = word + "TERMINATED |\n"; //useless
			break;
		}
	}
	std::cout << word;
	return word;
}

std::vector<PCB_ptr>& ProcessManager::getReadyQueue()
{
	return readyQueue;
}

std::vector<PCB_ptr>& ProcessManager::getProcessList()
{
	return processList;
}

int ProcessManager::getNextPID()
{
	int id = 0;
	bool ok = true;
	for (;;)
	{
		ok = true;
		for (auto pcb : processList)
		{
			if (pcb->processID == id)
			{
				id++;
				ok = false;
				break;
			}
		}
		if (ok) break;
	}
	return id;
}

void ProcessManager::deleteProcessFromList(int pid)
{
	if (processList.empty())
	{
		throw std::length_error("ProcessList is empty. You cannot delete a process.");
	}
	processList.erase(std::remove_if(std::begin(processList), std::end(processList),
	                                 [&pid](auto& pcb) { return pcb->processID == pid; }),
	                  std::end(processList));
}

void ProcessManager::deleteProcessFromQueue(int pid)
{
	if (readyQueue.empty())
	{
		throw std::length_error("ReadyQueue is empty. You cannot delete a process.");
	}
	readyQueue.erase(std::remove_if(std::begin(readyQueue), std::end(readyQueue),
	                                 [&pid](auto& pcb) { return pcb->processID == pid; }),
	                  std::end(readyQueue));
}

void ProcessManager::addProcessToList(std::shared_ptr<PCB> process)
{
	processList.push_back(process);
}

void ProcessManager::addProcessToQueue(std::shared_ptr<PCB> process)
{
	readyQueue.push_back(process);
}


PCB_ptr ProcessManager::getProcessFromList(std::string processName)
{
	if(processList.empty())
	{
		throw std::length_error("ProcessList is empty.");
	}
	
	for (int i = 0; i < this->processList.size(); i++)
	{
		if (this->processList[i]->processName == processName)
		{
			return this->processList.at(i);
		}
		else
		{
			throw std::invalid_argument("The process does not exist.");
		}
	}
}

void ProcessManager::changeStatusChosenProcess(int pid, PCBStatus sts)
{
	auto it = std::find_if(std::begin(processList), std::end(processList), [pid](PCB_ptr& pcb) { return pcb->getPID() == pid; });
	if (it == std::end(processList))
	{
		throw std::invalid_argument("The process does not exist.");
	}

	(*it)->changeStatus(sts);
}
