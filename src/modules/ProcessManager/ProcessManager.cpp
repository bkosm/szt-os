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

void ProcessManager::showChosenProcess(const PCB& process)
{
	std::cout << "| CHOSEN PROCESS |\n";
	std::cout << "| " << process.processName << " | " << process.processID << " | ";
	switch (process.status)
	{
	case PCBStatus::New:
		std::cout << "NEW |\n";
		break;
	case PCBStatus::Ready:
		std::cout << "READY |\n";
		break;
	case PCBStatus::Running:
		std::cout << "RUNNING |\n";
		break;
	case PCBStatus::Waiting:
		std::cout << "WAITING |\n";
		break;
	case PCBStatus::Terminated:
		std::cout << "TERMINATED |\n";  //useless
		break;
	}
}

void ProcessManager::showProcessList(std::vector<PCB> list)
{
	std::cout << "| PROCESS LIST |\n";
	for (auto const element : list)
	{
		std::cout << "| " << element.processName << " | " << element.processID << " | ";
		switch (element.status)
		{
		case PCBStatus::New:
			std::cout << "NEW |\n";
			break;
		case PCBStatus::Ready:
			std::cout << "READY |\n";
			break;
		case PCBStatus::Running:
			std::cout << "RUNNING |\n";
			break;
		case PCBStatus::Waiting:
			std::cout << "WAITING |\n";
			break;
		case PCBStatus::Terminated:
			std::cout << "TERMINATED |\n";  //useless
			break;
		}
	}
}

void ProcessManager::showReadyQueue(std::vector<PCB> queue)
{
	std::cout << "| READY QUEUE |\n";
	for (auto const element : queue)
	{
		std::cout << "| " << element.processName << " | " << element.processID << " | ";
		switch (element.status)
		{
		case PCBStatus::New:
			std::cout << "NEW |\n";
			break;
		case PCBStatus::Ready:
			std::cout << "READY |\n";
			break;
		case PCBStatus::Running:
			std::cout << "RUNNING |\n";
			break;
		case PCBStatus::Waiting:
			std::cout << "WAITING |\n";
			break;
		case PCBStatus::Terminated:
			std::cout << "TERMINATED |\n";  //useless
			break;
		}
	}
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

