#pragma once
#include <memory>

template <typename T>
using uPtr = std::unique_ptr<T>;

template <typename T>
using sPtr = std::shared_ptr<T>;

enum LockState {
    // Typ wyliczeniowy wartosci zwrotnych funkcji zamka.
    OK = 0,
    Locked = 1,
    Unlocked = 2,
    CantLock = 3,
    CantUnlock = 4,
    CantInit = 5
};
