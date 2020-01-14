#pragma once
#include <memory>
#include <vector>
<<<<<<< HEAD
#include <memory>

class Shell;
class PCB;

class Scheduler
{
	using PCB_ptr = std::shared_ptr<PCB>;
public:
	Scheduler(Shell *shell);

	void onReadyPcb(PCB_ptr readyPCB);
	void schedulePcb();
	PCB_ptr getRunningPcb();
    void setRunningPcb(PCB_ptr pcbPtr);
=======
#include "../ProcessManager/PCB.hpp"

class Shell;

class Scheduler {
    using PCB_ptr = std::shared_ptr<PCB>;

public:
    Scheduler(Shell *shell);

    void onReadyPcb(PCB_ptr readyPCB);
    void schedulePcb();
    PCB_ptr getRunningProcess();
>>>>>>> origin/roch

private:
    const float Alpha{ 0.5f };
    const uint8_t DefaultEstimatedTime{ 5 };

<<<<<<< HEAD
	const float Alpha{0.5f};
	const uint8_t DefaultEstimatedTime{5};

	Shell *shell;
	PCB_ptr runningPcb = nullptr;

	void updateEstimatedTime(PCB_ptr pcb);
=======
    Shell *shell;
    PCB_ptr runningProcess;

    void updateEstimatedTime(PCB_ptr pcb);
>>>>>>> origin/roch
};

