#include "LockManager.hpp"
#include "../../Shell.hpp"

LockManager::LockManager(Shell* shell) : shell_(shell)
{
}

Lock LockManager::createLock()
{
	return Lock();
}

void LockManager::deleteProcessFromLockQueue(std::shared_ptr<PCB> pcb) const
{
	std::vector<File>& fileList = shell_->getFileManager().getFiles();

	for (File& file : fileList)
	{
		std::vector<std::shared_ptr<PCB>>& processQueue = file.lock.getProcessQueue();
		auto itemIter = processQueue.begin();
		while (itemIter != processQueue.end())
		{
			if (pcb == *itemIter)
			{
				if (itemIter == processQueue.begin())
				{
					shell_->getFileManager().closeFile(file.name, pcb);

					if (!processQueue.empty())
					{
						file.lock.setLocked();
					}
				}
				else
				{
					processQueue.erase(itemIter);
				}
				break;
			}
			++itemIter;
		}
	}
}
