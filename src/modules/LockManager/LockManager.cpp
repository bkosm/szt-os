#include "LockManager.hpp"

LockManager::LockManager(Shell& shell) : shell_(shell)
{
}

Lock createLock(const bool startLocked)
{
	auto newLock = Lock();

	if (startLocked)
	{
		newLock.aquire();
	}

	return newLock;
}
