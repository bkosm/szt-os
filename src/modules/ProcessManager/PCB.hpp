#pragma once
#include <string>
#include <queue>
#include <list>

enum class PCBStatus
{
	New = 0,
	Waiting = 1,
	Ready = 2,
	Running = 3,
	Terminated = 4
};

class PCB
{
public:
	std::string processName;
	int processID;
	PCBStatus status;
	//Shell& shell;
	//===== PROCESSOR CONTROL ======
	uint8_t estimatedTime;
	uint8_t executedInstr;
	//===== INTEPRETER =====
	uint8_t AX, BX, CX, DX;
	uint8_t insnIndex;

	//===== MEMORY CONTROL =====
	//std::vector<PAGE> insnPages;
	//===== FILE CONTROL =====

	//===== LOCK CONTROL =====
	//Absolutna cisza w samym œrodku szalej¹cego ¿ywio³u. 

	PCB(std::string name, int ID, PCBStatus processstatus);
	~PCB();
	void showProcess();
};

class ProcessManager
{
	std::vector<PCB> processList;
	std::vector<PCB*> readyQueue;
public:
	PCB& createProcess(std::string name, std::string fileName);
	int getNextPID();

	std::vector<PCB*> getReadyQueue();
};

int randomPID(int min, int max);
void showChosenProcess(PCB process);