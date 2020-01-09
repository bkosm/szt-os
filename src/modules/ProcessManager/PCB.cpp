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

PCB::PCB(std::string name, int ID, PCBStatus processStatus)
	: processName(std::move(name)), processID(ID), status(processStatus) {
}

std::string PCB::showProcess() const
{
	std::string word;
	word = "| " + processName + " | " + std::to_string(processID) + " | ";
	switch (status)
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
	std::cout << word;
	return word;
}

int PCB::getPID() const
{
	return this->processID;
}

PCBStatus PCB::getStatus() const
{
	return this->status;
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