#pragma once
#include "PCB.hpp"
#include <vector>
#include <memory>
#include <string>

class Shell;

class ProcessManager {
	using PCB_ptr = std::shared_ptr<PCB>;

	std::vector<PCB_ptr> processList;
	std::vector<PCB_ptr> readyQueue;
public:
	ProcessManager(Shell *shell);
	~ProcessManager() = default;

	PCB_ptr createProcess(std::string name, std::string programName);
	PCB_ptr createDummyProcess();
	
	int getNextPID();
	Shell *shell;

	std::vector<PCB_ptr>& getReadyQueue();
	std::vector<PCB_ptr>& getProcessList();

	static std::string showChosenProcess(PCB_ptr process);
	std::string showProcessList();
	std::string showReadyQueue();
	std::string showPriority();

	void deleteProcessFromList(int pid);
	void deleteProcessFromQueue(int pid);

	void addProcessToList(PCB_ptr process);
	void addProcessToQueue(PCB_ptr process);

	PCB_ptr getProcessFromList(std::string processName);
	PCB_ptr getProcessFromList(int PID);

	PCBStatus convertStringToPCBStatus(std::string& processStatusName) const;


	void changeStatusChosenProcess(int pid, PCBStatus sts);
	/*
	 * processList - vector wszystkich procesow: nowych, oczekuj¹cych itd.
	 * readyQueue - vector procesow gotowych
	 *
	 * createProcess - metoda dodaj¹ca proces do listy wszystkich procesów
	 * getNextPID - metoda generuj¹ca niepowtarzalny numer ID
	 *
	 * getReadyQueue - metoda zwracaj¹ca referencjê do kolejki procesow gotowych
	 * getProcessList - metoda zwracaj¹ca referencjê do kolejki wszystkich procesow
	 *
	 * showChosenProcess - metoda wyœwietlaj¹ca proces o podanej nazwie
	 * showProcessList - metoda wyswietlajaca listê wszystkich procesów
	 * showReadyQueue - metoda wyswietlaj¹ca kolejkê procesow gotowych
	 * deleteProcessFromList - metoda usuwaj¹ca proces o podanym ID
	 * deleteProcessFromQueue - metoda usuwaj¹ca proces o podanym ID
	 * addProcessToList - metoda dodaj¹ca wybrany proces do listy
	 * getProcessFromList - metoda zwracaj¹ca proces o podanej nazwie
	 * changeStatusChosenProcess - metoda zmieniaja status procesu wybranego z listy
	 */
};
