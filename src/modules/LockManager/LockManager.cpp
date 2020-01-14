#include "LockManager.hpp"

LockManager::LockManager(Shell *shell) : shell_(shell)
{
}

Lock LockManager::createLock()
{
	return Lock();
}
