#include "Scheduler.hpp"
#include <algorithm>
#include <iterator>

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
}

void Scheduler::runProcess()
{
	auto& pcb = getCurrentProcess();
	//Shell.Interpreter.handleInst(pcb);
}
