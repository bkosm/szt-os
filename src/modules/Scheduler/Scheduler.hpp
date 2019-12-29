#pragma once
#include <vector>
#include <memory>
#include "../ProcessManager/PCB.hpp"
#include "../../Shell.hpp"

class Scheduler
{
	using PCB_ptr = std::shared_ptr<PCB>;
public:
	Scheduler(Shell &shell);

	void onReadyPcb(PCB_ptr readyPCB);
	void schedulePcb();
	PCB_ptr getRunningProcess();

private:

	const float Alpha{0.5f};
	const uint8_t DefaultEstimatedTime{5};

	Shell &shell;
	PCB_ptr runningProcess;

	void updateEstimatedTime(PCB_ptr pcb);
};
