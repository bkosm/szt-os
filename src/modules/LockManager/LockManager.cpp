#include "LockManager.hpp"

LockManager::LockManager(Shell& shell) : shell_(shell)
{
}

Lock createLock(const bool startLocked)
{
	Lock newLock;

	if (startLocked) newLock.aquire();

	return newLock;
}
