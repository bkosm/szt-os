#include "LockManager.hpp"

Lock createLock(bool startLocked) {
    Lock newLock = Lock();

    if (startLocked) {
        newLock.aquire();
    }

    return newLock;
}