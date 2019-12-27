#pragma once
#include <vector>
#include "../ProcessManager/PCB.hpp"
#include "../../Shell.hpp"

class Scheduler
{
public:
	Scheduler(Shell &shell);

	void updateReadyQueue();

private:

	const float Alpha{0.5f};
	const uint8_t DefaultEstimatedTime{5};
	std::vector<PCB*> readyProcesses;
	Shell &shell;

	uint8_t calculateEstimatedTime(PCB *pcb);
	void sortReadyProcesses();
};
