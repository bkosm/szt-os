#include "MemoryManager.h"
#include <fstream>

ProcessPageTableData::ProcessPageTableData(bool status, int frameNumber) : status(status), frameNumber(frameNumber)
{
}

ProcessPageTableData::ProcessPageTableData()
{
	this->status = false;
	this->frameNumber = -1;
}

MemoryManager::Page::Page() : Page("")
{
}

MemoryManager::Page::Page(std::string pageData)
{
	/*Uzupe�nianie argumentu spacjami, je�eli jest za ma�y*/
	while (pageData.length() < 16) { pageData += " "; }

	/*Przepisywanie argumentu do strony*/
	for (auto i = 0; i < 16; i++)
		this->pageData[i] = pageData[i];
}

void MemoryManager::Page::print() const
{
	for (auto& i : pageData)
	{
		if (i == ' ') std::cout << "_";
		else std::cout << i;
	}
	std::cout << '\n';
}

/*Lista ramek - konstruktor*/
MemoryManager::FrameListData::FrameListData(bool isFree, int processID, int pageID,
                                            std::vector<ProcessPageTableData>* processPageList)
	: isFree(isFree), processID(processID), pageID(pageID), processPageList(processPageList)
{
};

/*Konstruktor - RAM*/
MemoryManager::MemoryManager() : RAM{}
{
	for (auto i = 0; i < 32; i++)
	{
		this->Frames.emplace_back(FrameListData(true, -1, -1, nullptr));
	}
}

/*Dekonstruktor MemoryMenager'a*/
MemoryManager::~MemoryManager() = default;

/*Pokazuje obecn� zawarto�� pami�ci operacyjnej RAM*/
void MemoryManager::showMemory()
{
	std::cout << "RAM - CONTENT IN PHYSICAL MEMORY\n";
	for (int i = 0; i < 512; i++)
	{
		if (i % 16 == 0 && i != 0) std::cout << "\nFrame number " << i / 16 << ": \t";
		else if (i % 16 == 0) std::cout << "Frame number " << i / 16 << ": \t";
		if (RAM[i] == ' ' || RAM[i] == '\0') std::cout << '_';
		else if (RAM[i] == '\n' || RAM[i] == '\r') std::cout << '_';
		else std::cout << RAM[i];
	}
}

/*Pokazuje dany fragment pami�ci operacyjnej RAM*/
void MemoryManager::showMemory(int begin, int end)
{
	if (begin + end > 512)
	{
		std::cout << "Error: The number of bytes of memory to be analyzed is outside the memory range \n";
	}
	else
	{
		std::cout << "Displaying physical memory from cell " << begin << " to " << begin + end << ":\n";
		for (int i = begin; i < begin + end; i++)
		{
			if (i != 0 && i % 16 == 0) { std::cout << "\n"; }
			if (RAM[i] == ' ' || RAM[i] == '\0' || RAM[i] == '\n' || RAM[i] == '\r') std::cout << '-';
			else if (RAM[i] != ' ') std::cout << "test" << RAM[i];
		}
		std::cout << std::endl;
	}
}

/*Pokazuje obecn� zawarto�� stosu*/
void MemoryManager::showStack()
{
	std::cout << "First in first out Stack: ";
	for (auto frame : lastUsedFramesStack)
	{
		std::cout << frame << " ";
	}
	std::cout << std::endl;
}

/*Pokazuje zawarto�� ramki*/
void MemoryManager::showFrame()
{
	std::cout << "FRAMES INFO: \n";
	std::cout << "\t\tFREE \tPAGE \tPID " << std::endl;
	int i = 0;
	for (auto& frame : Frames)
	{
		std::cout << "Frame no." << i++ << ":\t" << frame.isFree << "\t" << frame.pageID << "\t" << frame.processID <<
			"\n";
	}
}


/*Funkcje u�ytkowe RAM*/

/*Akutalizacja stosu*/
void MemoryManager::updateStack(int frameNumber)
{
	if (frameNumber > 31) return;

	for (auto it = lastUsedFramesStack.begin(); it != lastUsedFramesStack.end(); ++it)
	{
		if (*it == frameNumber)
		{
			lastUsedFramesStack.erase(it);
			break;
		}
	}

	lastUsedFramesStack.push_back(frameNumber);
}


/*Process bezczynno�ci umieszczany w RAMie przy starcie systemu*/
void MemoryManager::processOfInaction()
{
	for (auto& cell : RAM)
	{
		cell = ' ';
	}
	PageFile.emplace(1, std::vector<Page>{Page("JMP [0];")}); /*Wykonanie skoku do adresu 0*/
}


int MemoryManager::loadProgram(const std::string& filePath, int PID)
{
	std::fstream file(filePath); /*Plik na dysku*/
	std::string helpVariable; /*Zmienna pomocnicza*/
	std::string program; /*Program w jednej linii*/
	std::vector<Page> pageVector; /*wektor stron do dodania*/

	if (!file.is_open())
	{
		std::cout << "Nie mozna odtworzyc pliku!\n";
		return -1;
	}

	while (std::getline(file, helpVariable))
	{
		/*Dodanie na ko�cu znacznika ko�ca linii*/
		if (!helpVariable.empty())
		{
			helpVariable += ';';
			program += helpVariable;
		}
	}

	const double pageSize = ceil(program.length() / 16.0);
	helpVariable.clear();

	/*Dzielenie programu na stronnice*/
	for (char i : program)
	{
		helpVariable += i;
		if (helpVariable.size() == 16)
		{
			pageVector.emplace_back(Page(helpVariable));
			helpVariable.clear();
		}
	}

	/*Sprawdzanie czy zmienna przechowuje jeszcze jakie� instrukcje*/
	if (!helpVariable.empty()) { pageVector.emplace_back(Page(helpVariable)); }
	helpVariable.clear();

	if (pageSize * 16 < 16 * pageVector.size())
	{
		std::cout << "Error: proces ma przypisane zbyt malo pamieci!\n";
		return -1;
	}


	//Sprawdzanie, czy program nie potrzebuje wiecej stronic w pami�ci
	for (int i = pageVector.size(); i < pageSize; i++)
		pageVector.emplace_back(helpVariable);

	//Dodanie stronic do pliku wymiany
	PageFile.insert(make_pair(PID, pageVector));

	return program.length();
}

/*Usuwanie programu z pamieci*/
void MemoryManager::deleteProgram(int PID)
{
	for (size_t i = 0; i < Frames.size(); i++)
	{
		if (Frames[i].processID == PID)
		{
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

/*Znajduje pierwsz� woln� ramke w pami�ci RAM*/
int MemoryManager::searchFreeFrame()
{
	int seekedFrame = -1;


	for (size_t i = 0; i < Frames.size(); i++)
	{
		if (Frames[i].isFree)
		{
			seekedFrame = i;
			break;
		}
	}
	return seekedFrame;
}

/*Zamienia stronnice zgodnie z algorytmem LRU*/
int MemoryManager::swapPage(int pageID, int PID)
{
	/*Numer ramki, kt�ra b�dzie zamieniana*/
	const int Frame = *lastUsedFramesStack.begin();
	/*Przepisuje ramk� do pliku wymiany*/
	for (int i = Frame * 16; i < Frame * 16 + 16; i++)
	{
		PageFile[Frames[Frame].processID][Frames[Frame].pageID].pageData[i - (Frame * 16)] = RAM[i];
	}

	/*z*/
	Frames[Frame].processPageList->at(Frames[Frame].pageID).status = false;
	Frames[Frame].processPageList->at(Frames[Frame].pageID).frameNumber = -1;

	return Frame;
}


/*�aduje stronnice do pamieci RAM*/
int MemoryManager::loadPagesToMemory(Page page, int pageID, int PID,
                                     const std::shared_ptr<std::vector<ProcessPageTableData>>& pageList)
{
	int frame = searchFreeFrame();

	if (frame == -1) { frame = swapPage(pageID, PID); }

	//Przepisywanie stronicy do pami�ci RAM
	int it = 0;
	const int end = frame * 16 + 16;
	for (int i = frame * 16; i < end; i++)
		RAM[i] = page.pageData[it++];

	//Zmienianie bit'u w indeksie wymiany stronic
	pageList->at(pageID).status = true;
	pageList->at(pageID).frameNumber = frame;

	//Aktualizacja stosu u�ywalno�ci
	updateStack(frame);

	//Aktualizacja informacji o ramce
	Frames[frame].isFree = false;
	Frames[frame].pageID = pageID;
	Frames[frame].processID = PID;
	Frames[frame].processPageList = pageList;

	return frame;
}


/*Tworzy wska�nik do tablicy stronic procesu - wywo�ywane w momencie tworzenia procesu*/
std::shared_ptr<std::vector<ProcessPageTableData>> MemoryManager::createProcessPageList(int memoryNeeded, int PID)
{
	const double pages = ceil(static_cast<double>(memoryNeeded) / 16);
	std::shared_ptr<std::vector<ProcessPageTableData>> pageList = std::make_shared<std::vector<ProcessPageTableData>>();

	for (int i = 0; i < pages; i++)
	{
		pageList->push_back(ProcessPageTableData(false, 0));
	}


	//Za�adowanie pierszej stronicy naszego programu do Pami�ci RAM
	loadPagesToMemory(PageFile[PID].at(0), 0, PID, pageList);

	return pageList;
}
