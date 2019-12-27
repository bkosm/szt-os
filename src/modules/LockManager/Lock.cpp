#include "Lock.hpp"

Lock::Lock() { locked_ = false; }

bool Lock::aquire()
{
	if (locked_ == false) locked_ = true;

	return locked_;
}

bool Lock::unlock()
{
	if (locked_ == true) locked_ = false;

	return locked_;
}

std::vector<std::shared_ptr<PCB>>& Lock::getProcessQueue()
{
	return processQueue_;
}

std::string Lock::getProcessQueueString() const
{
	std::string queueString = "Processes requesting the file:\n";

	unsigned number = 1;
	for (const auto& pcb : processQueue_)
	{
		queueString += std::to_string(number) + ":\tName: " + pcb->processName + "\tPID: " + std::to_string(pcb->processID) + "\n";
		++number;
	}

	return queueString;
}
