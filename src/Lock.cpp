#include "Lock.hpp"

int Lock::init(uPtr<File>& file)
{
    if (!file->lock) {
        file->lock = std::make_unique<Lock>();

        file->lock->file_index = file->block_index;
        file->lock->locked = false;

        return LockState::OK;
    }

    return LockState::CantInit;
}

int Lock::aquire(uPtr<File>& file)
{
    if (file->lock->file_index == file->block_index
        && !file->lock->locked) {
        file->lock->locked = true;
        return LockState::OK;
    }

    return LockState::CantLock;
}

int Lock::unlock(uPtr<File>& file)
{
    if (file->lock->file_index == file->block_index
        && file->lock->locked) {
        file->lock->locked = false;
        return LockState::OK;
    }

    return LockState::CantUnlock;
}
