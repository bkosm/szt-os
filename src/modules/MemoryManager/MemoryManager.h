#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <fstream>
#include <math.h>
#include <iostream>

class PCB;
class Shell;

struct ProcessPageTableData {
	bool status; /*True - ramka znajduje siê w pamieæi ram, False - ramka nie znajduje siê w pamiêci ram*/
	int frameNumber; /*Numer ramki w której znajduje siê stronica*/

	ProcessPageTableData();
	ProcessPageTableData(bool status, int frameNumber);
};

class MemoryManager {
public:
	Shell* shell;
	
	unsigned char RAM[512]; /*Pamiêæ fizyczna komputera*/
/*private:*/
	/*Struktura pojedyñczej strony*/
	struct Page {
		char pageData[16] = { ' ' };

		Page();
		explicit Page(std::string pageData);

		void print() const;
	};

	/*Lista ramek*/

	struct FrameListData {
		bool isFree; /*pokazuje czy ramka jest wolna*/
		int processID; /*numer procesu*/
		int pageID; /*numer strony*/
		std::shared_ptr<std::vector<ProcessPageTableData>> processPageList; /*tablica stronic procesu znajduj¹cego siê w PCB*/

		FrameListData(bool isFree, int processID, int pageID, std::vector<ProcessPageTableData>* processPageList);
	};

	/*Ramki za³adowane w pamiêci fizycznej RAM*/

	std::vector<FrameListData> Frames;

	/*Stos ramek u¿ywanych w osttanim czasie*/

	std::list<int> lastUsedFramesStack { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 };

	/*Plik stronicowania*/

	std::map<int, std::vector<Page>> PageFile;
	/*public:*/
	MemoryManager(Shell *shell);
	~MemoryManager();

	std::string showMemory() /*const*/; /*Pokazuje obecn¹ zawartoœæ pamiêci operacyjnej RAM*/
	std::string showMemory(int begin, int end)/*const*/; /*Pokazuje dany fragment pamiêci operacyjnej RAM*/
	std::string showStack(); /*Pokazuje obecn¹ zawartoœæ stosu*/
	std::string showFrame(); /*Pokazuje zawartoœæ ramki*/

/*Funkcje u¿ytkowe RAM*/

	/*Aktualizacja stosu*/
	void updateStack(int frameNumber);

	/*Process bezczynnoœci umieszczany w RAMie przy starcie systemu*/
	void processOfInaction();

	/*Funkcja ³aduj¹ca program do pliku wymiany*/
	int loadProgram(const std::string& filePath, int PID);

	/*Znajduje pierwsz¹ woln¹ ramke w pamiêci RAM*/
	int searchFreeFrame();

	/*£aduje stronnice do pamieci RAM*/
	int loadPagesToMemory(Page page, int pageID, int PID, const std::shared_ptr<std::vector<ProcessPageTableData>>& pageList);

	/*Usuwanie programu z pamieci*/
	void deleteProgram(int PID);

	/*Tworzy wskaŸnik do tablicy stronic procesu - wywo³ywane w momencie tworzenia procesu*/
	std::shared_ptr<std::vector<ProcessPageTableData>> createProcessPageList(int memoryNeeded, int PID);

	/*Zamienia stronnice zgodnie z algorytmem LRU*/
	int swapPage(int pageID, int PID);

	/*Edycja instrukcji procesu*/
	void setByte(PCB &pcb, uint8_t data, int target);

	/*Pobranie instrukcji procesu od danego momentu*/
	uint8_t getByte(std::shared_ptr<PCB> &pcb, int target);

};