#pragma once
#include "Definitions.hpp"


class Lock {
private:
    bool locked;

public:
    Lock();
    ~Lock() = default;

    bool aquire();
    /* Funkcja blokujaca zamek na podanym pliku.
     *
     * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
     */
    bool unlock();
    /* Funkcja odblokowujaca zamek z pliku.
     * 
     * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
     */
};