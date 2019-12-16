#include "ProcessControl.hpp"
#include <random>
#include <memory>


PCB::PCB(std::string name, int ID, PCBStatus processstatus) {
	std::cout << "Process was created!\n";
	processName = name;
	processID = ID;
	status = processstatus;
}

PCB::~PCB()
{
	std::cout << "Process was terminated!\n";
}

PCB& ProcessManager::createProcess(std::string name, std::string fileName)
{
	//std::vector<PCB> queue;
	//queue.add(PCB("xD", 0, PCBStatus::New));
	processList.emplace_back("XD", getNextPID(), PCBStatus::New);
	return processList.back();
}

void PCB::showProcess()
{
	std::cout << "| " << processName << " | " << processID << " | ";
	switch (status)
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



void showChosenProcess(PCB process)
{
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

void showProcessList(std::list<PCB> list)
{
	for (int i = 0; i < list.size(); i++)
	{

	}
}

std::vector<PCB*> ProcessManager::getReadyQueue()
{
	return readyQueue;
}

int ProcessManager::getNextPID()
{
	int id = 0;

	bool ok = true;
	for (;;)
	{
		ok = true;
		for (PCB pcb : processList)
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

int randomPID(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

