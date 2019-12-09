#pragma once
#include "Lock.hpp"

class LockManager {
public:
    LockManager() = default;
    ~LockManager() = default;

    Lock createLock(bool startLocked = false);
    /* Funkcja tworzaca nowy zamek.
     *
     * @param startLocked w jakim stanie ma byc utworzony zamek.
     * @return Nowy obiekt zamka.
     */
};