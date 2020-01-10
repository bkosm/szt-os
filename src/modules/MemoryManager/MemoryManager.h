#pragma once
#include <cstdint>
#include "../ProcessManager/PCB.hpp"

class Shell;

class MemoryManager
{
public:
	MemoryManager(Shell* shell);

	void loadDummy(PCB& pcb);
	void loadProgram(PCB& pcb, const std::string& programName);
	void deleteProgram(PCB& pcb);

	uint8_t getByte(PCB& pcb, uint16_t target);
	void setByte(PCB& pcb, uint16_t target, uint8_t data);

	std::string showMemory() const;

private:
	uint8_t RAM[512] = { 0 };
	bool usedFrames[32] = { false };

	Shell* shell;

	std::vector<uint8_t> getFreeFrames();
};