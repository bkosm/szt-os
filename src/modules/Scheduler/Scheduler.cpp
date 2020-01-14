#include "Scheduler.hpp"
#include <algorithm>
#include <exception>
#include <iterator>
#include <exception>
#include <system_error>
#include "../../Shell.hpp"
#include "../ProcessManager/PCB.hpp"

Scheduler::Scheduler(Shell *shell) : shell(shell) {
}

void Scheduler::onReadyPcb(PCB_ptr readyPcb) {
    if (runningPcb->getStatus() != PCBStatus::Dummy)
        updateEstimatedTime(runningPcb);

	if (readyPcb->getStatus() != PCBStatus::Dummy)
		updateEstimatedTime(readyPcb);
}

std::shared_ptr<PCB> Scheduler::getRunningPcb() {
    return runningPcb;
}

void Scheduler::setRunningPcb(PCB_ptr pcbPtr) {
    runningPcb = pcbPtr;
}

void Scheduler::updateEstimatedTime(PCB_ptr pcb) {
    auto previousEstimatedTime = pcb->estimatedTime;
    pcb->estimatedTime = previousEstimatedTime == 0 ?
        DefaultEstimatedTime : pcb->estimatedTime = Alpha * pcb->insnCounter + (1 - Alpha) * pcb->estimatedTime;
}

void Scheduler::schedulePcb() {
    std::vector<PCB_ptr> readyProcesses = shell->getProcessManager().getReadyQueue();
    
    for (auto &pcbPtr : readyProcesses) {
		if (pcbPtr->getStatus() == PCBStatus::Running)
		{
            pcbPtr->changeStatus(PCBStatus::Ready);
            break;
		}
    }

    std::stable_sort(std::begin(readyProcesses), std::end(readyProcesses),
                     [](auto &lhs, auto &rhs) {
                         return lhs->estimatedTime < rhs->estimatedTime;
                     });

	runningPcb = readyProcesses.front();
	if (runningPcb->getStatus() == PCBStatus::Ready)
		runningPcb->changeStatus(PCBStatus::Running);
}

