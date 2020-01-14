#include "Scheduler.hpp"
#include <algorithm>
#include <iterator>
#include "../../Shell.hpp"
#include "../ProcessManager/PCB.hpp"

constexpr auto INT(const float x) { return static_cast<int>(x); }
constexpr auto FLOAT(const int x) { return static_cast<float>(x); }

Scheduler::Scheduler(Shell* shell) : shell(shell)
{
}

void Scheduler::onReadyQueueChange()
{
	updateAllEstimatedTimes();
	chooseRunningPcb();
}

std::shared_ptr<PCB> Scheduler::getRunningPcb() const
{
	return runningPcb;
}

void Scheduler::setRunningPcb(PCB_ptr pcbPtr)
{
	runningPcb = pcbPtr;
}

uint8_t Scheduler::getDefaultEstimatedTime() const
{
	return DefaultEstimatedTime;
}

void Scheduler::updateEstimatedTime(const PCB_ptr& pcb) const
{
	if (pcb->insnCounter == 0
		|| pcb->getPID() == 0)
	{
		return;
	}

	const auto previousEstimatedTime = pcb->estimatedTime;
	pcb->estimatedTime = previousEstimatedTime == 0
		                     ? DefaultEstimatedTime
		                     : std::min(INT(Alpha * FLOAT(pcb->insnCounter) + (1 - Alpha) * FLOAT(pcb->estimatedTime)),
		                                254);

	pcb->insnCounter = 0;
}

void Scheduler::updateAllEstimatedTimes() const
{
	auto readyProcesses = shell->getProcessManager().getReadyQueue();

	for (auto& pcb : readyProcesses)
	{
		updateEstimatedTime(pcb);
	}
}

void Scheduler::chooseRunningPcb()
{
	auto readyProcesses = shell->getProcessManager().getReadyQueue();

	for (auto& pcb : readyProcesses)
	{
		if (pcb->getStatus() == PCBStatus::Running)
		{
			pcb->changeStatus(PCBStatus::Ready);
			break;
		}
	}

	std::stable_sort(std::begin(readyProcesses), std::end(readyProcesses),
	                 [](const auto& lhs, const auto& rhs)
	                 {
		                 return lhs->estimatedTime < rhs->estimatedTime;
	                 });

	runningPcb = readyProcesses.front();
	runningPcb->changeStatus(PCBStatus::Running);
}
