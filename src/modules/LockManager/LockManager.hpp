#pragma once
#include "Lock.hpp"

class Shell;

class LockManager
{
public:
	explicit LockManager(Shell *shell);
	~LockManager() = default;

	Lock createLock(bool startLocked = false) const;
	/* Funkcja tworzaca nowy zamek.
	 *
	 * @param startLocked w jakim stanie ma byc utworzony zamek.
	 * @return Nowy obiekt zamka.
	 */

private:
	Shell *shell_;
};
