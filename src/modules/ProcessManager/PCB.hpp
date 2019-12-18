#pragma once
#include <string>
#include <random>

enum class PCBStatus {
	Error = -1,
	New = 0,
	Waiting = 1,
	Ready = 2,
	Running = 3,
	Terminated = 4
};

class PCB {
public:
	std::string processName;
	int processID;
	PCBStatus status;
	//Shell& shell;
	//===== PROCESSOR CONTROL ======
	int t;
	int tau;
	//===== INTEPRETER =====
	uint8_t AX, BX, CX, DX;
	uint8_t insnIndex;
	uint8_t insnCounter;
	uint8_t estimatedTime;
	//===== MEMORY CONTROL =====
	//std::vector<PAGE> insnPages;
	//===== FILE CONTROL =====

	//===== LOCK CONTROL =====
	//Absolutna cisza w samym œrodku szalej¹cego ¿ywio³u. 

	PCB();
	PCB(std::string name, int ID, PCBStatus processStatus);

	~PCB();

	void showProcess() const;
	int getPID() const;
	void changeStatus(PCBStatus status);
};

int randomPID(int min, int max);