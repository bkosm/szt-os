#pragma once
#include <string>
#include <vector>
<<<<<<< HEAD

struct PCB
{
	uint8_t estimatedTime{5};
	uint8_t executedInstr{0};
};

class Scheduler
{
public:
	Scheduler();
=======
#include "../ProcessManager/PCB.hpp"

class Scheduler {
public:
    Scheduler(std::vector<PCB> &readyProcesses);
>>>>>>> origin/roch

	std::string toString();
	void updateReadyQueue();
	PCB& getCurrentProcess();
	void runProcess();

private:
<<<<<<< HEAD
	const uint8_t alpha;
	std::vector<PCB>& readyProcesses;

	void calculateEstimatedTime(PCB& pcb);
	void sortReadyProcesses();
=======
    const float Alpha{ 0.5f };
    const uint8_t DefaultEstimatedTime{ 5 };
    std::vector<PCB> &readyProcesses;

    uint8_t calculateEstimatedTime(PCB &pcb);
    void sortReadyProcesses();
>>>>>>> origin/roch
};
