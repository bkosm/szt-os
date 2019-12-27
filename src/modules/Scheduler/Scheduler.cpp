#include "Scheduler.hpp"
#include <algorithm>
#include <iterator>

<<<<<<< HEAD
void Scheduler::calculateEstimatedTime(PCB& pcb)
{
	pcb.estimatedTime = pcb.executedInstr * alpha + (1 - alpha) * pcb.estimatedTime;
}

void Scheduler::updateReadyQueue()
{
	for (auto pcb : readyProcesses)
	{
		calculateEstimatedTime(pcb);
	}
	sortReadyProcesses();
}

void Scheduler::sortReadyProcesses()
{
	std::stable_sort(std::begin(readyProcesses), std::end(readyProcesses),
	                 [](auto& lfs, auto& rhs)
	                 {
		                 return lhs.estimatedTime <= rhs.estimatedTime;
	                 });
=======
Scheduler::Scheduler(std::vector<PCB> &readyProcesses) : readyProcesses(readyProcesses) {
    updateReadyQueue();
}

void Scheduler::updateReadyQueue() {
    for (auto pcb : readyProcesses) {
        pcb.estimatedTime = pcb.estimatedTime == 0 ? DefaultEstimatedTime : calculateEstimatedTime(pcb);
    }
    sortReadyProcesses();
}

uint8_t Scheduler::calculateEstimatedTime(PCB &pcb) {
    return pcb.executedInstr * Alpha + (1 - Alpha) * pcb.estimatedTime;
}

void Scheduler::sortReadyProcesses() {
    std::stable_sort(std::begin(readyProcesses), std::end(readyProcesses),
                     [](auto &lhs, auto &rhs) {
                         return lhs.estimatedTime <= rhs.estimatedTime;
                     });
>>>>>>> origin/roch
}

void Scheduler::runProcess()
{
	auto& pcb = getCurrentProcess();
	//Shell.Interpreter.handleInst(pcb);
}
