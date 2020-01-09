#include "MemoryManager.h"
#include "../ProcessManager/PCB.hpp"
#include "../../Shell.hpp"
#include <iostream>
#include <sstream>

ProcessPageTableData::ProcessPageTableData(bool status, int frameNumber) : status(status), frameNumber(frameNumber) {}

ProcessPageTableData::ProcessPageTableData() {
	this->status = false;
	this->frameNumber = -1;
}

MemoryManager::Page::Page() :Page("") {}

MemoryManager::Page::Page(std::string pageData) {
	/*Uzupełnianie argumentu spacjami, jeżeli jest za mały*/
	while (pageData.length() < 16) { pageData += " "; }

	/*Przepisywanie argumentu do strony*/
	for (auto i = 0; i < 16; i++)
		this->pageData[i] = pageData[i];
}

void MemoryManager::Page::print() const {
	for (auto& i : pageData) {
		if (i == ' ') std::cout << "_";
		else std::cout << i;
	}
	std::cout << '\n';
}

/*Lista ramek - konstruktor*/
MemoryManager::FrameListData::FrameListData(bool isFree, int processID, int pageID, std::vector<ProcessPageTableData>* processPageList)
	: isFree(isFree), processID(processID), pageID(pageID), processPageList(processPageList) {};

/*Konstruktor - RAM*/
MemoryManager::MemoryManager(Shell *shell) : shell(shell), RAM {} {
	for (auto i = 0; i < 32; i++) {
		this->Frames.emplace_back(FrameListData(true, -1, -1, nullptr));
	}
}

/*Dekonstruktor MemoryMenager'a*/
MemoryManager::~MemoryManager() = default;

/*Pokazuje obecną zawartość pamięci operacyjnej RAM*/
std::string MemoryManager::showMemory() {
	std::ostringstream stream;
	stream << "RAM - CONTENT IN PHYSICAL MEMORY\n";
	for (auto i = 0; i < 512; i++) {
		if (i % 16 == 0 && i != 0) stream << "\nFrame number " << i / 16 << ": \t";
		else if (i % 16 == 0) stream << "Frame number " << i / 16 << ": \t";
		if (RAM[i] == ' ' || RAM[i] == '\0') stream << '_';
		else if (RAM[i] == '\n' || RAM[i] == '\r') stream << '_';
		else stream << RAM[i];
	}
	return stream.str();
}

/*Pokazuje dany fragment pamięci operacyjnej RAM*/
std::string MemoryManager::showMemory(int begin, int end) {
	std::ostringstream stream;
	if (begin + end > 512) {
		stream << "Error: The number of bytes of memory to be analyzed is outside the memory range \n";
	}
	else {
		stream << "Displaying physical memory from cell " << begin << " to " << begin + end << ":\n";
		for (int i = begin; i < begin + end; i++) {
			if (i != 0 && i % 16 == 0) { stream << "\n"; }
			if (RAM[i] == ' ' || RAM[i] == '\0' || RAM[i] == '\n' || RAM[i] == '\r') stream << '-';
			else if (RAM[i] != ' ') stream << "test" << RAM[i];
		}
		stream << std::endl;
	}
	return stream.str();
}

/*Pokazuje obecną zawartość stosu*/
std::string MemoryManager::showStack() {
	std::ostringstream stream;
	stream << "First in first out Stack: ";
	for (auto frame : lastUsedFramesStack) {
		stream << frame << " ";
	}
	stream << std::endl;
	return stream.str();
}

/*Pokazuje zawartość ramki*/
std::string MemoryManager::showFrame() {
	std::ostringstream stream;
	stream << "FRAMES INFO: \n";
	stream << "\t\tFREE \tPAGE \tPID " << std::endl;
	int i = 0;
	for (auto& frame : Frames) {
		stream << "Frame no." << i++ << ":\t" << frame.isFree << "\t" << frame.pageID << "\t" << frame.processID << "\n";
	}
	return stream.str();
}


/*Funkcje użytkowe RAM*/

/*Akutalizacja stosu*/
void MemoryManager::updateStack(int frameNumber) {
	if (frameNumber > 31) return;

	for (auto it = lastUsedFramesStack.begin(); it != lastUsedFramesStack.end(); ++it) {
		if (*it == frameNumber) {
			lastUsedFramesStack.erase(it);
			break;
		}
	}

	lastUsedFramesStack.push_back(frameNumber);
}


/*Process bezczynności umieszczany w RAMie przy starcie systemu*/
void MemoryManager::processOfInaction() {
	for (auto& cell : RAM) {
		cell = ' ';
	}
	PageFile.emplace(1, std::vector<Page>{Page("JMP [0];")}); /*Wykonanie skoku do adresu 0*/
}


int MemoryManager::loadProgram(const std::string &name, int PID) {
	std::string helpVariable;		/*Zmienna pomocnicza*/
	std::string program;			/*Program w jednej linii*/
	std::vector<Page> pageVector;	/*wektor stron do dodania*/

	try
	{
		program = shell->getInterpreter().loadProgram(name);
	} catch ()
	{
		throw;
	}
	
	const double pageSize = ceil(program.length() / 16.0);

	/*Dzielenie programu na stronnice*/
	for (char i : program) {
		helpVariable += i;
		if (helpVariable.size() == 16) {
			pageVector.emplace_back(Page(helpVariable));
			helpVariable.clear();
		}
	}

	/*Sprawdzanie czy zmienna przechowuje jeszcze jakieś instrukcje*/
	if (!helpVariable.empty()) { pageVector.emplace_back(Page(helpVariable)); }
	helpVariable.clear();

	if (pageSize * 16 < 16 * pageVector.size()) {
		std::cout << "Error: proces ma przypisane zbyt malo pamieci!\n";
		return -1;
	}


	//Sprawdzanie, czy program nie potrzebuje wiecej stronic w pamięci
	for (int i = pageVector.size(); i < pageSize; i++)
		pageVector.emplace_back(helpVariable);

	//Dodanie stronic do pliku wymiany
	PageFile.insert(make_pair(PID, pageVector));

	// shell->getProcessManager().getProcessFromList(PID)->processPageList->

	return program.length();
}

/*Usuwanie programu z pamieci*/
void MemoryManager::deleteProgram(int PID) {
	for (size_t i = 0; i < Frames.size(); i++) {
		if (Frames[i].processID == PID) {
			for (size_t j = i * 16; j < i * 16 + 16; j++)
				RAM[j] = ' ';
			updateStack(i);
			Frames[i].isFree = true;
			Frames[i].pageID = -1;
			Frames[i].processID = -1;
			PageFile.erase(PID);
		}
	}
}

/*Znajduje pierwszą wolną ramke w pamięci RAM*/
int MemoryManager::searchFreeFrame() {
	int seekedFrame = -1;


	for (size_t i = 0; i < Frames.size(); i++) {
		if (Frames[i].isFree) {
			seekedFrame = i;
			break;
		}
	}
	return seekedFrame;
}

/*Zamienia stronnice zgodnie z algorytmem LRU*/
int MemoryManager::swapPage(int pageID, int PID) {
	/*Numer ramki, która będzie zamieniana*/
	const int Frame = *lastUsedFramesStack.begin();
	/*Przepisuje ramkę do pliku wymiany*/
	for (int i = Frame * 16; i < Frame * 16 + 16; i++) {
		PageFile[Frames[Frame].processID][Frames[Frame].pageID].pageData[i - (Frame * 16)] = RAM[i];
	}

	/*z*/
	Frames[Frame].processPageList->at(Frames[Frame].pageID).status = false;
	Frames[Frame].processPageList->at(Frames[Frame].pageID).frameNumber = -1;

	return Frame;
}


/*Ładuje stronnice do pamieci RAM*/
int MemoryManager::loadPagesToMemory(Page page, int pageID, int PID, const std::shared_ptr<std::vector<ProcessPageTableData>> &pageList) {
	int frame = searchFreeFrame();

	if (frame == -1) { frame = swapPage(pageID, PID); }

	//Przepisywanie stronicy do pamięci RAM
	int it = 0;
	const int end = frame * 16 + 16;
	for (int i = frame * 16; i < end; i++)
		RAM[i] = page.pageData[it++];

	//Zmienianie bit'u w indeksie wymiany stronic
	pageList->at(pageID).status = true;
	pageList->at(pageID).frameNumber = frame;

	//Aktualizacja stosu używalności
	updateStack(frame);

	//Aktualizacja informacji o ramce
	Frames[frame].isFree = false;
	Frames[frame].pageID = pageID;
	Frames[frame].processID = PID;
	Frames[frame].processPageList = pageList;

	return frame;
}


/*Tworzy wskaźnik do tablicy stronic procesu - wywoływane w momencie tworzenia procesu*/
std::shared_ptr<std::vector<ProcessPageTableData>> MemoryManager::createProcessPageList(int memoryNeeded, int PID) {
	const double pages = ceil(static_cast<double>(memoryNeeded) / 16);
	std::shared_ptr<std::vector<ProcessPageTableData>> pageList = std::make_shared<std::vector<ProcessPageTableData>>();

	for (int i = 0; i < pages; i++) {
		pageList->push_back(ProcessPageTableData(false, 0));
	}


	//Załadowanie pierszej stronicy naszego programu do Pamięci RAM
	loadPagesToMemory(PageFile[PID].at(0), 0, PID, pageList);

	return pageList;
}

void MemoryManager::setByte(std::shared_ptr<PCB> &pcb, std::string data, int address) {
	if (!data.empty()) {
		if (address + data.length() - 1 <= pcb->processPageList->size() * 16 && address >= 0) {
			for (int i = 0; i < data.length(); i++) {
				int pageID = int(floor((address + 1) / 16.0));
				if (pcb->processPageList->at(pageID).status == 0) {
					loadPagesToMemory(PageFile[pcb->processID][pageID], pageID, pcb->processID, pcb->processPageList);
				}
				RAM[pcb->processPageList->at(pageID).frameNumber * 16 + address + i - (16 * pageID)] = data[i];

			}
		}
		else {
			throw std::out_of_range("Bledne odwolanie do pamieci.");
		}
	}
}

/*NIE WIEM CZY O TO CHODZIŁO - NIE WIEM CZY DZIAŁA*/
/*Pobranie instrukcji procesu od danego momentu*/
uint8_t MemoryManager::getByte(std::shared_ptr<PCB> &pcb, int target) {
	uint8_t temp = 0;
	unsigned int page_id = target / 16;

	/*przekroczenie zakres dla tego procesu !!!*/

	/*Numer stronicy w pamięci*/
	page_id = target / 16;
	/*sprawdzenie czy znajduje się w pamieci operacyjnej*/
	if (!pcb->processPageList->at(page_id).status)
		loadPagesToMemory(PageFile[pcb->processID][page_id], page_id, pcb->processID, pcb->processPageList);

	if (pcb->processPageList->at(page_id).status) {
		const int Frame = pcb->processPageList->at(page_id).frameNumber;//Bieżąco używana ramka

		temp += RAM[Frame * 16 + target - 16 * page_id];
	}
	return temp;
}



