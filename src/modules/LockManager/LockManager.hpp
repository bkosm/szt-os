#pragma once
#include "Lock.hpp"

class Shell;

class LockManager
{
public:
	explicit LockManager(Shell *shell);
	~LockManager() = default;

	void deleteProcessFromLockQueue(std::shared_ptr<PCB> pcb) const;
	/* Funkcja usuwajaca proces z kolejek wszystkich zamkow.
	*
	* @param pcb Proces ktory nalezy usunac z kolejek.
	*/
	static Lock createLock();
	/* Funkcja tworzaca nowy zamek.
	 *
	 * @param startLocked w jakim stanie ma byc utworzony zamek.
	 * @return Wskaznik na nowy obiekt zamka.
	 */

private:
	Shell *shell_;
};
