#include "MemoryManager.h"
#include "../ProcessManager/PCB.hpp"
#include "../../Shell.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

constexpr uint8_t FRAME_SIZE = 16;
constexpr uint8_t MAX_FRAMES = 32;

MemoryManager::MemoryManager(Shell* shell) : shell(shell) {}

void MemoryManager::loadProgram(PCB& pcb, const std::string& programName)
{
	std::string programCode;
	try
	{
		programCode = shell->getInterpreter().loadProgram(programName);
	}
	catch (std::invalid_argument & e)
	{
		throw e;
	}

	uint8_t neededPages = 1 + ((programCode.length() - 1) / FRAME_SIZE);
	std::vector<uint8_t> freeFrames = getFreeFrames();

	if (neededPages > freeFrames.size())
	{
		throw std::overflow_error("Program nie miesci sie w pamieci.");
	}

	int programIndex = 0;
	for (int i = 0; i < neededPages; ++i)
	{
		usedFrames[freeFrames[i]] = true;
		pcb.pages.push_back(freeFrames[i]);

		for (int index = 0; index < FRAME_SIZE; ++index)
		{
			if (programIndex >= programCode.length()) return;

			RAM[freeFrames[i] * FRAME_SIZE + index] = programCode[programIndex];
			++programIndex;
		}
	}
}

void MemoryManager::deleteProgram(PCB& pcb)
{
	for (uint8_t frame : pcb.pages)
	{
		usedFrames[frame] = false;

		for (int i = 0; i < FRAME_SIZE; ++i)
		{
			RAM[frame * FRAME_SIZE + i] = 0;
		}
	}
}

uint8_t MemoryManager::getByte(PCB& pcb, uint16_t target)
{
	uint8_t pageNum = target / FRAME_SIZE;
	if (pageNum >= pcb.pages.size())
	{
		throw std::out_of_range("Proces odwolal sie do blednej stronicy.");
	}

	uint8_t frameNum = pcb.pages[pageNum];

	if (frameNum >= MAX_FRAMES)
	{
		throw std::out_of_range("Proces odwolal sie do blednej ramki.");
	}

	uint8_t byteNum = (frameNum * FRAME_SIZE) + (target % FRAME_SIZE);
	return RAM[byteNum];
}

void MemoryManager::setByte(PCB& pcb, uint16_t target, uint8_t data)
{
	uint8_t pageNum = target / FRAME_SIZE;
	if (pageNum >= pcb.pages.size())
	{
		throw std::out_of_range("Proces odwolal sie do blednej stronicy.");
	}

	uint8_t frameNum = pcb.pages[pageNum];

	if (frameNum >= MAX_FRAMES)
	{
		throw std::out_of_range("Proces odwolal sie do blednej ramki.");
	}

	RAM[(frameNum * FRAME_SIZE) + (target % FRAME_SIZE)] = data;;
}

std::string MemoryManager::showMemory() const
{
	std::ostringstream output;

	unsigned frameIndex = 0, pageNumber = 1, column = 0;

	output << "RAM CONTENT:" << std::endl;
	for (auto ch : RAM)
	{
		if (column == 2) {
			if (frameIndex == 0)
			{
				output << std::setfill('0') << std::setw(2) << pageNumber << ": {" << ch;
				++pageNumber;
			}
			else if (frameIndex == 15)
			{
				output << ch << "}" << std::endl;
				frameIndex = -1;
				column = 0;
			}
			else
			{
				output << ch;
			}
			++frameIndex;
		}
		else {
			if (frameIndex == 0)
			{
				output << std::setfill('0') << std::setw(2) << pageNumber << ": {" << ch;
				++pageNumber;
			}
			else if (frameIndex == 15)
			{
				output << ch << "} ";
				frameIndex = -1;
				++column;
			}
			else
			{
				output << ch;
			}
			++frameIndex;
		}
	}

	return output.str();
}

std::vector<uint8_t> MemoryManager::getFreeFrames()
{
	std::vector<uint8_t> frames;

	for (uint8_t i = 0; i < MAX_FRAMES; ++i)
	{
		if (usedFrames[i] == false) frames.push_back(i);
	}

	return frames;
}
