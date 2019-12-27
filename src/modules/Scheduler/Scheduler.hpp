#pragma once
#include <string>
#include <vector>
#include "../ProcessManager/PCB.hpp"

class Scheduler
{
public:
	Scheduler(std::vector<PCB>& readyProcesses);

	std::string toString();
	void updateReadyQueue();
	PCB& getCurrentProcess();
	void runProcess();

private:

	const float Alpha{0.5f};
	const uint8_t DefaultEstimatedTime{5};
	std::vector<PCB>& readyProcesses;

	uint8_t calculateEstimatedTime(PCB& pcb);
	void sortReadyProcesses();
};
