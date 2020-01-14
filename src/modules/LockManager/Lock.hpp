#pragma once

#include "../ProcessManager/PCB.hpp"
#include <vector>
#include <memory>

class Lock
{
public:
	Lock();
	~Lock() = default;

	bool aquire(std::shared_ptr<PCB>& pcb);
	/* Funkcja blokujaca zamek na pliku.
	 *
	 * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
	 */
	bool unlock(std::shared_ptr<PCB>& pcb);
	/* Funkcja odblokowujaca zamek z pliku.
	 *
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
	std::vector<std::shared_ptr<PCB>> processQueue_;

	bool locked_;
};
