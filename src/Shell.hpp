#pragma once
#include "modules/Scheduler/Scheduler.hpp"
#include "modules/Interpreter/Interpreter.hpp"
#include "modules/ProcessManager/ProcessManager.hpp"
#include "modules/MemoryManager/MemoryManager.h"
#include "modules/FileManager/FileManager.h"
#include "modules/LockManager/LockManager.hpp"

class Shell
{
	Interpreter interpreter;
	Scheduler scheduler;
	ProcessManager processManager;
	MemoryManager memoryManager;
	FileManager fileManager;
	LockManager lockManager;

public:
	Shell();
	virtual ~Shell() = default;

	Interpreter& getInterpreter();
	Scheduler& getScheduler();
	ProcessManager& getProcessManager();
	MemoryManager& getMemoryManager();
	LockManager& getLockManager();
	FileManager& getFileManager();
};
