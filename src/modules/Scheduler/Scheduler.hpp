#pragma once
#include <memory>
#include <vector>
#include <memory>

class Shell;
class PCB;

class Scheduler
{
	using PCB_ptr = std::shared_ptr<PCB>;
public:
	Scheduler(Shell* shell);

	void onReadyQueueChange();
	uint8_t getDefaultEstimatedTime() const;
	PCB_ptr getRunningPcb() const;
	void setRunningPcb(PCB_ptr pcbPtr);

private:
	const float Alpha{ 0.5f };
	const uint8_t DefaultEstimatedTime{ 5 };

	Shell* shell;
	PCB_ptr runningPcb = nullptr;

	void updateEstimatedTime(const PCB_ptr& pcb) const;
	void updateAllEstimatedTimes() const;
	void chooseRunningPcb();
};

