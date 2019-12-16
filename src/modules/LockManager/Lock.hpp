#pragma once

class Lock
{
private:
	bool locked_;

public:
	Lock();
	~Lock() = default;

	bool aquire();
	/* Funkcja blokujaca zamek na pliku.
	 *
	 * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
	 */
	bool unlock();
	/* Funkcja odblokowujaca zamek z pliku.
	 *
	 * @return Wynik dzialania funkcji, false oznacza niepowodzenie.
	 */
};
