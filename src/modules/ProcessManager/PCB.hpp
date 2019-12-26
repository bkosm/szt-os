#pragma once
#include <string>
<<<<<<< HEAD
#include <queue>
#include <list>

enum class PCBStatus
{
=======
#include <random>

enum class PCBStatus {
	Error = -1,
>>>>>>> origin/hubert
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

<<<<<<< HEAD
class ProcessManager
{
	std::vector<PCB> processList;
	std::vector<PCB*> readyQueue;
public:
	PCB& createProcess(std::string name, std::string fileName);
	int getNextPID();
=======
	~PCB();
>>>>>>> origin/hubert

	void showProcess() const;
	int getPID() const;
	void changeStatus(PCBStatus status);
};

<<<<<<< HEAD
int randomPID(int min, int max);
void showChosenProcess(PCB process);
=======
int randomPID(int min, int max);
>>>>>>> origin/hubert
