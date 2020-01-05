#include "Lock.hpp"
#include <sstream>
#include <iomanip>

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
	std::ostringstream output;
	output << "Processes requesting the file:\n";

	unsigned number = 0;
	for (const auto& pcb : processQueue_)
	{
		++number;
		output << std::setfill('0') << std::setw(5) << std::to_string(number)
			<< " - Name: " << pcb->processName
			<< "\t| PID: " + std::to_string(pcb->processID) << std::endl;
	}

	return number == 0 ? output.str() += "~none~\n" : output.str();
}
