#pragma once
#include "Lock.hpp"
#include "../Interpreter/Interpreter.hpp"

class LockManager
{
public:
	LockManager(Shell& shell);
	~LockManager() = default;

	Lock createLock(bool startLocked = false);
	/* Funkcja tworzaca nowy zamek.
	 *
	 * @param startLocked w jakim stanie ma byc utworzony zamek.
	 * @return Nowy obiekt zamka.
	 */
	
private:
	Shell& shell_;
};
