#pragma once
#include "PCB.hpp"
#include <vector>
#include <memory>
#include <string>

class ProcessManager {
	std::vector<PCB> processList;
	std::vector<PCB> readyQueue;
public:
	void createProcess(std::string name, std::string fileName);
	int getNextPID();

	std::vector<PCB>& getReadyQueue();
	std::vector<PCB>& getProcessList();

	void showChosenProcess(const PCB& process);
	static void showProcessList(std::vector<PCB> list);

	void deleteProcessFromList(int pid);

	void addProcessToList(PCB process);

	PCB& getProcessFromList(std::string processName);
};
