#pragma once

class Lock
{
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

private:
	bool locked_;
};
