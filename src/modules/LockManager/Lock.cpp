#include "Lock.hpp"

Lock::Lock() { this->locked = false; }

bool Lock::aquire()
{
    if (this->locked == false) {
        this->locked = true;
        return true;
    }

    return false;
}

bool Lock::unlock()
{
    if (this->locked == true) {
        this->locked = false;
        return true;
    }

    return false;
}
