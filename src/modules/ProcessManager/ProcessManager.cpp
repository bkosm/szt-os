#include "PCB.hpp"
#include "ProcessManager.hpp"
#include "../../Shell.hpp"
#include "../../SztosException.hpp"
#include <random>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <sstream>

using PCB_ptr = std::shared_ptr<PCB>;

ProcessManager::ProcessManager(Shell* shell) : shell(shell)
{
}


PCB_ptr ProcessManager::createProcess(std::string name, std::string programName)
{
	auto pcb = std::make_shared<PCB>(std::move(name), getNextPID(), PCBStatus::New);
	addProcessToList(pcb);

	try {
		shell->getMemoryManager().loadProgram(*pcb, programName);
	}
	catch (SztosException & e) {
		deleteProcessFromList(pcb->getPID());
		throw e;
	}

	pcb->estimatedTime = shell->getScheduler().getDefaultEstimatedTime();
	pcb->changeStatus(PCBStatus::Ready);
	addProcessToQueue(pcb);
	return pcb;
}

PCB_ptr ProcessManager::createDummyProcess()
{
	auto pcb = std::make_shared<PCB>("dummy", getNextPID(), PCBStatus::New);
	addProcessToList(pcb);

	try {
		shell->getMemoryManager().loadDummy(*pcb);
	}
	catch (SztosException & e) {
		deleteProcessFromList(pcb->getPID());
		throw e;
	}

	pcb->estimatedTime = UINT8_MAX;
	pcb->changeStatus(PCBStatus::Ready);
	addProcessToQueue(pcb);
	return pcb;
}

std::string ProcessManager::showChosenProcess(PCB_ptr process)
{
	std::string word;
	word = "| CHOSEN PROCESS |\n";
	word = word + "| " + process->processName + " | " + (char)process->processID + " | ";
	switch (process->status)
	{
	case PCBStatus::New:
		word = word + "NEW |\n";
		break;
	case PCBStatus::Ready:
		word = word + "READY |\n";
		break;
	case PCBStatus::Running:
		word = word + "RUNNING |\n";
		break;
	case PCBStatus::Waiting:
		word = word + "WAITING |\n";
		break;
	case PCBStatus::Terminated:
		word = word + "TERMINATED |\n"; //useless
		break;
	}
	return word;
}

std::string ProcessManager::showProcessList()
{
	std::ostringstream output;
	unsigned number = 1;

	output << "PROCESS LIST:" << std::endl;
	for (auto const& element : processList)
	{
		output << std::setfill('0') << std::setw(2) << number
			<< " N : " << element->processName << "\tID: " << element->processID
			<< "\tAX: " << int(element->AX) << " BX: " << int(element->BX)
			<< " CX: " << int(element->CX) << " DX: " << int(element->DX) << std::endl
			<< "~~ ET: " << int(element->estimatedTime) << "\t\tIC: " << int(element->insnCounter) << "\tII: " << int(element->insnIndex)
			<< "\tST: ";
		switch (element->status)
		{
		case PCBStatus::New:
			output << "NEW\n";
			break;
		case PCBStatus::Ready:
			output << "READY\n";
			break;
		case PCBStatus::Running:
			output << "RUNNING\n";
			break;
		case PCBStatus::Waiting:
			output << "WAITING\n";
			break;
		case PCBStatus::Terminated:
			output << "TERMINATED\n";
			break;
		case PCBStatus::Error:
			output << "ERROR\n";
			break;
		default: break;
		}
		++number;

		output << std::endl;
	}

	return output.str();
}

std::string ProcessManager::showReadyQueue()
{
	std::ostringstream output;
	unsigned number = 1;

	output << "READY PROCESS QUEUE:" << std::endl;
	for (auto const& element : readyQueue)
	{
		output << std::setfill('0') << std::setw(2) << number
			<< " N : " << element->processName << "\tID: " << element->processID
			<< "\tST: ";
		switch (element->status)
		{
		case PCBStatus::New:
			output << "NEW\n";
			break;
		case PCBStatus::Ready:
			output << "READY\n";
			break;
		case PCBStatus::Running:
			output << "RUNNING\n";
			break;
		case PCBStatus::Waiting:
			output << "WAITING\n";
			break;
		case PCBStatus::Terminated:
			output << "TERMINATED\n";
			break;
		case PCBStatus::Error:
			output << "ERROR\n";
			break;
		default: break;
		}
		++number;
	}

	return output.str();
}

std::string ProcessManager::showPriority()
{
	std::ostringstream stream;
	stream << "PRIORITY OF PROCESSES:\n";
	for (const auto& element : processList)
	{
		stream
			<< element->processID << "\tN: " << element->processName
			<< "\tET: " << int(element->estimatedTime) << "\n";
	}
	return stream.str();
}

std::string ProcessManager::showProcessPages(int pid) {
	if (processList.empty())
	{
		throw SztosException("Process list is empty.");
	}
	if (pid >= processList.size() || processList.size() < 0)
	{
		throw SztosException("Out of range.");
	}

	auto process = getProcessFromList(pid);
	std::ostringstream ss;

	ss << "Process uses pages: ";
	for (auto page : process->pages) {
		ss << int(page) << " ";
	}
	ss << std::endl;

	return ss.str();
}

std::vector<PCB_ptr>& ProcessManager::getReadyQueue()
{
	return readyQueue;
}

std::vector<PCB_ptr>& ProcessManager::getProcessList()
{
	return processList;
}

int ProcessManager::getNextPID()
{
	int id = 0;
	bool ok = true;
	for (;;)
	{
		ok = true;
		for (auto pcb : processList)
		{
			if (pcb->processID == id)
			{
				id++;
				ok = false;
				break;
			}
		}
		if (ok) break;
	}
	return id;
}

void ProcessManager::deleteProcessFromList(int pid)
{
	if (processList.empty())
	{
		throw SztosException("ProcessList is empty. You cannot delete a process.");
	}

	try
	{
		deleteProcessFromQueue(pid);
	}
	catch (SztosException&) {}

	try
	{
		auto ptr = getProcessFromList(pid);
		shell->getLockManager().deleteProcessFromLockQueue(ptr);
	}
	catch (SztosException & e)
	{
		throw;
	}

	size_t prevSize = processList.size();
	processList.erase(std::remove_if(std::begin(processList), std::end(processList),
		[&pid](auto& pcb) { return pcb->processID == pid; }),
		std::end(processList));

	if (prevSize == processList.size())
	{
		throw SztosException("The process with the given ID does not exist.");
	}
}

void ProcessManager::deleteProcessFromQueue(int pid)
{
	if (readyQueue.empty())
	{
		throw SztosException("ReadyQueue is empty. You cannot delete a process.");
	}

	size_t prevSize = readyQueue.size();
	auto it = readyQueue.erase(std::remove_if(std::begin(readyQueue), std::end(readyQueue),
		[&pid](auto& pcb) { return pcb->processID == pid; }),
		std::end(readyQueue));

	if (prevSize == readyQueue.size())
	{
		throw SztosException("The process with the given ID does not exist in ready queue.");
	}

	shell->getScheduler().onReadyQueueChange();
}

void ProcessManager::addProcessToList(std::shared_ptr<PCB> process)
{
	processList.push_back(process);
}

void ProcessManager::addProcessToQueue(std::shared_ptr<PCB> process)
{
	readyQueue.push_back(process);
	shell->getScheduler().onReadyQueueChange();
}


PCB_ptr ProcessManager::getProcessFromList(std::string processName)
{
	if (processList.empty())
	{
		throw SztosException("ProcessList is empty.");
	}

	for (int i = 0; i < this->processList.size(); i++)
	{
		if (this->processList[i]->processName == processName)
		{
			return this->processList.at(i);
		}
		else
		{
			throw SztosException("The process does not exist.");
		}
	}
}

PCB_ptr ProcessManager::getProcessFromList(int PID)
{
	if (processList.empty())
	{
		throw SztosException("ProcessList is empty.");
	}

	auto pcb = std::find_if(std::begin(processList), std::end(processList), [PID](const auto& pcb)
		{
			return pcb->getPID() == PID;
		});

	if (pcb == std::end(processList))
	{
		throw SztosException("The process does not exist.");
	}

	return *pcb;
}

PCBStatus ProcessManager::convertStringToPCBStatus(std::string& processStatusName) const
{
	std::transform(processStatusName.begin(), processStatusName.end(), processStatusName.begin(),
		[](unsigned char c) { return std::tolower(c); });


	if (processStatusName != "ready" && processStatusName != "waiting" && processStatusName != "terminated")
	{
		throw SztosException("The status provided is not available to the user.");
	}
	if (processStatusName == "ready") {
		return PCBStatus::Ready;
	}
	if (processStatusName == "waiting") {
		return PCBStatus::Waiting;
	}
	if (processStatusName == "terminated") {
		return PCBStatus::Terminated;
	}
}

void ProcessManager::changeStatusChosenProcess(int pid, PCBStatus sts)
{
	auto it = std::find_if(std::begin(processList), std::end(processList), [pid](PCB_ptr& pcb) { return pcb->getPID() == pid; });
	if (it == std::end(processList))
	{
		throw SztosException("The process does not exist.");
	}

	(*it)->changeStatus(sts);
}
