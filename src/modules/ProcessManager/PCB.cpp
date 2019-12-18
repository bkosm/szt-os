#include "PCB.hpp"
#include <random>
#include <memory>
#include <iostream>
#include <utility>

PCB::PCB()
{
	this->processName = "error";
	this->processID = -1;
	this->status = PCBStatus::Error;
}

PCB::PCB(std::string name, int ID, PCBStatus processStatus) {
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
		std::cout << "TERMINATED |\n";  //useless
		break;
	}
}

int PCB::getPID() const
{
	return this->processID;
}

void PCB::changeStatus(PCBStatus status)
{
	this->status = status;
}

int randomPID(int min, int max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(min, max);
	return dis(gen);
}

