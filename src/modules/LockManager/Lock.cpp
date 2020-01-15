#include "Lock.hpp"
#include <sstream>
#include <iomanip>

Lock::Lock() { locked_ = false; }

bool Lock::aquire(std::shared_ptr<PCB> pcb)
{
	if (locked_ == false) {
		processQueue_.push_back(pcb);

		locked_ = true;
		return true;
	}

	if (pcb != nullptr) {
		processQueue_.push_back(pcb);
	}

	return false;
}

bool Lock::unlock(std::shared_ptr<PCB> pcb)
{
	if (locked_ == true and !processQueue_.empty()) {
		if (processQueue_.at(0) == pcb)
		{
			processQueue_.erase(processQueue_.begin());
			locked_ = false;
			return true;
		}
	}
	return false;
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

		if (pcb == nullptr)
		{
			output << std::setfill('0') << std::setw(5) << std::to_string(number)
				<< " - INTERFACE" << std::endl;
			continue;
		}

		output << std::setfill('0') << std::setw(5) << std::to_string(number)
			<< " - Name: " << pcb->processName
			<< "\t| PID: " + std::to_string(pcb->processID) << std::endl;
	}

	return number == 0 ? output.str() += "~none~\n" : output.str();
}

bool Lock::getState() const
{
	return locked_;
}
