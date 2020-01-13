#include "Shell.hpp"

Shell::Shell() :
	interpreter(this),
	scheduler(this),
	processManager(this),
	memoryManager(this),
	fileManager(this),
	lockManager(this)
{
}

Interpreter& Shell::getInterpreter()
{
	return interpreter;
}

Scheduler& Shell::getScheduler()
{
	return scheduler;
}

ProcessManager& Shell::getProcessManager()
{
	return processManager;
}

MemoryManager& Shell::getMemoryManager()
{
	return memoryManager;
}

LockManager& Shell::getLockManager()
{
	return lockManager;
}

FileManager& Shell::getFileManager()
{
	return fileManager;
}
