#include "Scheduler.hpp"
#include <algorithm>
#include <iterator>
#include <exception>
#include <system_error>
#include "../../Shell.hpp"

Scheduler::Scheduler(Shell *shell) : shell(shell) {
}

void Scheduler::onReadyPcb(PCB_ptr readyPcb) {
    if (runningProcess == nullptr)
        throw std::runtime_error("runningProcess should never be nullptr");
    
    if (runningProcess->status != PCBStatus::Dummy)
        updateEstimatedTime(runningProcess);

    updateEstimatedTime(readyPcb);
}

std::shared_ptr<PCB> Scheduler::getRunningProcess() {
    return runningProcess;
}

void Scheduler::updateEstimatedTime(PCB_ptr pcb) {
    auto previousEstimatedTime = pcb->estimatedTime;
    pcb->estimatedTime = previousEstimatedTime == 0 ?
        DefaultEstimatedTime : pcb->estimatedTime = Alpha * pcb->insnCounter + (1 - Alpha) * pcb->estimatedTime;
}

void Scheduler::schedulePcb() {
    std::vector<PCB_ptr> readyProcesses = shell->getProcessManager().getReadyQueue();

    std::stable_sort(std::begin(readyProcesses), std::end(readyProcesses),
                     [](auto &lhs, auto &rhs) {
                         return lhs->estimatedTime < rhs->estimatedTime;
                     });

    runningProcess = readyProcesses.front();
}

