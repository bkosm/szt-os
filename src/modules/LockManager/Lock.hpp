#pragma once

#include "../ProcessManager/PCB.hpp"
#include <vector>
#include <memory>

class LockManager;

class Lock
{
public:
	Lock();
	~Lock() = default;

	bool aquire(std::shared_ptr<PCB> pcb);
	/* Funkcja blokujaca zamek na pliku.
	 *
	 * @param pcb wskaznik na process ktory zada pliku.
	 * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
	 */
	bool unlock(std::shared_ptr<PCB> pcb);
	/* Funkcja odblokowujaca zamek z pliku.
	 *
	 * @param pcb wskaznik na process ktory zwalnia plik.
	 * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
	 */
	std::vector<std::shared_ptr<PCB>>& getProcessQueue();
	/* Funkcja zwracajaca kolejke procesow czekajaca na plik.
	 *
	 * @return Referencja do kolejki.
	 */
	std::string getProcessQueueString() const;
	/* Funkcja zwaracaja lancuch tekstowy sformatowany do wyswietlenia.
	 *
	 * @return Lancuch tekstowy.
	 */
	bool getState() const;
	/* Zwraca aktualny stan zamka.
	 *
	 * @return Zmienna boolowska
	 */

private:
	void setLocked();
	/* Ustawia zamek bezwarunkowo na zamkniety, potrzebna do zarzadzania kolejka
	 * zamkow przez Lock Managera.
	 */
	std::vector<std::shared_ptr<PCB>> processQueue_;

	bool locked_;

	/* Wymagane do zabezpieczenia enkapsulacji funkcji setLocked */
	friend class LockManager;
};
