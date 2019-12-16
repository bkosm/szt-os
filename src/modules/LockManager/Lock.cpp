#include "Lock.hpp"

Lock::Lock() { this->locked_ = false; }

bool Lock::aquire()
{
    if (this->locked_ == false) {
        this->locked_ = true;
        return true;
    }

    return false;
}

bool Lock::unlock()
{
    if (this->locked_ == true) {
        this->locked_ = false;
        return true;
    }

    return false;
}
