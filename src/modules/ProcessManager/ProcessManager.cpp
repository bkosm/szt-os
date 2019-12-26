#include "ProcessControl.hpp"
#include "ProcessManager.hpp"
#include <random>
#include <iostream>
#include <utility>

void ProcessManager::createProcess(std::string name, std::string fileName)
{
	PCB pcb(std::move(name), getNextPID(), PCBStatus::New);
	processList.push_back(pcb);
}

std::string ProcessManager::showChosenProcess(const PCB& process)
{
	std::string word;
	word = "| CHOSEN PROCESS |\n";
	word = word + "| " + process.processName + " | " + (char)process.processID + " | ";
	switch (process.status)
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
		word = word + "TERMINATED |\n";  //useless
		break;
	}
	return word;
}

std::string ProcessManager::showProcessList(std::vector<PCB> list)
{
	std::string word;
	word = "| PROCESS LIST |\n";
	for (auto const element : list)
	{
		word = word + "| " + element.processName + " | " + std::to_string(element.processID) + " | ";
		switch (element.status)
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
			word = word + "TERMINATED |\n";  //useless
			break;
		}
	}
	std::cout << word;
	return word;
}

std::string ProcessManager::showReadyQueue(std::vector<PCB> queue)
{
	std::string word;
	word = "| READY QUEUE |\n";
	for (auto const element : queue)
	{
		word = word + "| " + element.processName + " | " + std::to_string(element.processID) + " | ";
		switch (element.status)
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
			word = word + "TERMINATED |\n";  //useless
			break;
		}
	}
	std::cout << word;
	return word;
}

std::vector<PCB>& ProcessManager::getReadyQueue()
{
	return readyQueue;
}

std::vector<PCB>& ProcessManager::getProcessList()
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
		for (const PCB& pcb : processList)
		{
			if (pcb.processID == id)
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
	auto iter = this->processList.begin();

	while (iter != this->processList.end())
	{
		if (iter->processID == pid)
		{
			this->processList.erase(iter);
			break;
		}

		++iter;
	}
}

void ProcessManager::addProcessToList(PCB process)
{
	processList.push_back(process);
}

void ProcessManager::addProcessToQueue(PCB process)
{
	readyQueue.push_back(process);
}


PCB& ProcessManager::getProcessFromList(std::string processName)
{
	for (int i = 0; i < this->processList.size(); i++)
	{
		if (this->processList[i].processName == processName)
		{
			return this->processList.at(i);
		}
	}
}

void ProcessManager::changeStatusChosenProcess(int pid, PCBStatus sts)
{
	for (auto& pcb : processList)
	{
		if (pcb.processID == pid)
		{
			pcb.changeStatus(sts);
		}
	}
}

