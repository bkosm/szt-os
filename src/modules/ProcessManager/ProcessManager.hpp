#pragma once
#include "ProcessControl.hpp"
#include <vector>
#include <memory>
#include <string>

class ProcessManager {
	std::vector<PCB> processList;
	std::vector<PCB> readyQueue;
public:
	void createProcess(std::string name, std::string fileName);
	int getNextPID();

	std::vector<PCB>& getReadyQueue();
	std::vector<PCB>& getProcessList();

	void showChosenProcess(const PCB& process);
	static void showProcessList(std::vector<PCB> list);
	static void showReadyQueue(std::vector<PCB> queue);

	void deleteProcessFromList(int pid);

	void addProcessToList(PCB process);
	void addProcessToQueue(PCB process);

	PCB& getProcessFromList(std::string processName);

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
	 * addProcessToList - metoda dodaj¹ca wybrany proces do listy
	 * getProcessFromList - metoda zwracaj¹ca proces o podanej nazwie
	 * TO DO
	 * changeStatusChosenProcess - metoda zmieniaja status procesu wybranego z listy
	 */
};
