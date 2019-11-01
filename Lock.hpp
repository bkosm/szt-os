#pragma once
#include "Definitions.hpp"
#include <memory>

// Pogladowe wymagania dotyczace pliku.
struct File;
// TODO usunac File -------------------

class Lock {
private:
    bool locked;
    unsigned file_index;

public:
    Lock() = default;
    ~Lock() = default;

    static int init(uPtr<File>& file);

    /* Funkcja inicjalizujaca pola zamka w zaleznosci
     * od podanego pliku.
     *
     * @param file Wskaznik na obiekt pliku.
     * @return Kod bledu, 0 oznacza brak.
     */

    static int aquire(uPtr<File>& file);

    /* Funkcja blokujaca zamek na podanym pliku.
     *
     * @param file Wskaznik na obiekt pliku.
     * @return Kod bledu, 0 oznacza brak.
     */

    static int unlock(uPtr<File>& file);

    /* Funkcja odblokowujaca zamek z pliku.
     *
     * @param file Wskaznik na obiekt pliku.
     * @return Kod bledu, 0 oznacza brak.
     */
};

// Struktura pogladowa.
struct File {
    unsigned block_index;
    uPtr<Lock> lock;

    File(const unsigned id)
    {
        this->block_index = id;
    }
};
// --------------------
