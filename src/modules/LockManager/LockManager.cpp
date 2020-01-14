#include "LockManager.hpp"

LockManager::LockManager(Shell *shell) : shell_(shell)
{
}

Lock LockManager::createLock(const bool startLocked) const
{
	auto newLock = Lock();

	if (startLocked) newLock.aquire();

	return newLock;
}
