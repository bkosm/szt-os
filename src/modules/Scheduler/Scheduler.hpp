#pragma once

#include <string>
#include <vector>

<<<<<<< HEAD
class Scheduler
{
};
=======
struct PCB {
    uint8_t estimatedTime{5};
    uint8_t executedInstr{0};
};

class Scheduler {
   public:
    Scheduler();

    std::string toString();
    void updateReadyQueue();
    PCB &getCurrentProcess();
    void runProcess();

   private:
    const uint8_t alpha;
    std::vector<PCB> &readyProcesses;

    void calculateEstimatedTime(PCB &pcb);
    void sortReadyProcesses();
};
>>>>>>> origin/roch
