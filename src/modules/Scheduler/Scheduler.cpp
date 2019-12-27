#include "Scheduler.hpp"
#include <algorithm>
#include <iterator>


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
    return {}; //brakuje pola executed Instr pcb.executedInstr * Alpha + (1 - Alpha) * pcb.estimatedTime;
}

void Scheduler::sortReadyProcesses() {
    std::stable_sort(std::begin(readyProcesses), std::end(readyProcesses),
                     [](auto &lhs, auto &rhs) {
                         return lhs.estimatedTime <= rhs.estimatedTime;
                     });
}

void Scheduler::runProcess()
{
	//auto& pcb = getCurrentProcess();
	//Shell.Interpreter.handleInst(pcb);
}
