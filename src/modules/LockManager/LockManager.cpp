#include "LockManager.hpp"

LockManager::LockManager(Shell *shell) : shell_(shell)
{
}

Lock LockManager::createLock(const bool startLocked) const
{
	Lock newLock;

	if (startLocked) newLock.aquire();

	return newLock;
}
