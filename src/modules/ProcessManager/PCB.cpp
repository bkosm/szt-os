#include "PCB.hpp"
#include <random>
<<<<<<< HEAD
#include <iostream>
=======
#include <memory>
#include <iostream>
#include <utility>
>>>>>>> origin/hubert

PCB::PCB()
{
	this->processName = "error";
	this->processID = -1;
	this->status = PCBStatus::Error;
}

<<<<<<< HEAD
PCB::PCB(std::string name, int ID, PCBStatus processStatus)
{
=======
PCB::PCB(std::string name, int ID, PCBStatus processStatus) {
>>>>>>> origin/hubert
	std::cout << "Process was created!\n";
	processName = std::move(name);
	processID = ID;
	status = processStatus;
}

PCB::~PCB()
{
	//std::cout << "Process was terminated!\n";
}

void PCB::showProcess() const
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
		std::cout << "TERMINATED |\n"; //useless
		break;
	}
}

<<<<<<< HEAD

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
		std::cout << "TERMINATED |\n"; //useless
		break;
	}
}

void showProcessList(std::list<PCB> list)
{
	for (int i = 0; i < list.size(); i++)
	{
	}
=======
int PCB::getPID() const
{
	return this->processID;
>>>>>>> origin/hubert
}

void PCB::changeStatus(PCBStatus status)
{
	this->status = status;
}

int randomPID(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}
