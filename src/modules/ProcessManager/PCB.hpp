#pragma once
#include "../MemoryManager/MemoryManager.h"
#include <string>
#include <random>
#include <memory>

enum class PCBStatus
{
	Error = -1,
	New = 0,
	Waiting = 1,
	Ready = 2,
	Running = 3,
	Terminated = 4,
	Dummy = 5
};

class PCB
{
public:
	std::string processName;
	int processID;
	PCBStatus status;
	//===== PROCESSOR CONTROL ======
	int t;
	int tau;
	//===== INTEPRETER =====
	uint8_t AX, BX, CX, DX;
	uint8_t insnIndex;
	uint8_t insnCounter;
	uint8_t estimatedTime;
	//===== MEMORY CONTROL =====
	std::shared_ptr<std::vector<ProcessPageTableData>> processPageList=nullptr; /*tablica stronic procesu znajdującego się w PCB*/
	unsigned int sizePageProcess = 16; /*rozmiar strony*/
	//===== FILE CONTROL =====

	//===== LOCK CONTROL =====
	//Absolutna cisza w samym �rodku szalej�cego �ywio�u. 

	PCB();
	PCB(std::string name, int ID, PCBStatus processStatus);

	~PCB();

	std::string showProcess() const;
	int getPID() const;
	PCBStatus getStatus() const;
	void changeStatus(PCBStatus status);
	/*
	 * PCB - konstruktor
	 *
	 * showProcess - metoda wy�wietlaj�ca w�asno�ci wybranego procesu
	 * getPID - metoda zwracaj�ca ID wybranego procesu
	 * changeStatus - metoda zmieniaj�ca status wybranego procesu
	 */
};

int randomPID(int min, int max);
