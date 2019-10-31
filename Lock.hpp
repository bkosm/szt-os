#pragma once
#include <memory>

// Pogladowe wymagania dotyczace pliku.
struct File;
// TODO usunac File -------------------

enum LockState {
    // Typ wyliczeniowy wartosci zwrotnych.
    OK = 0,
    Locked = 1,
    Unlocked = 2,
    CantLock = 3,
    CantUnlock = 4,
    CantInit = 5
};

class Lock {
private:
    bool locked;
    unsigned file_index;

public:
    Lock() = default;
    ~Lock() = default;

    static int init(std::unique_ptr<File>& file);

    /* Funkcja inicjalizujaca pola zamka w zaleznosci
     * od podanego pliku.
     *
     * @param file Wskaznik na obiekt pliku.
     * @return Kod bledu, 0 oznacza brak.
     */

    static int aquire(std::unique_ptr<File>& file);

    /* Funkcja blokujaca zamek na podanym pliku.
     *
     * @param file Wskaznik na obiekt pliku.
     * @return Kod bledu, 0 oznacza brak.
     */

    static int unlock(std::unique_ptr<File>& file);

    /* Funkcja odblokowujaca zamek z pliku.
     *
     * @param file Wskaznik na obiekt pliku.
     * @return Kod bledu, 0 oznacza brak.
     */
};

// Struktura pogladowa.
struct File {
    unsigned block_index;
    std::unique_ptr<Lock> lock;

    File(const unsigned id)
    {
        this->block_index = id;
    }
};
// --------------------
