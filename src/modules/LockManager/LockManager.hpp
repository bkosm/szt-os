#pragma once
#include "Lock.hpp"
#include <memory>

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
	 * @return Wskaznik na nowy obiekt zamka.
	 */

private:
	Shell *shell_;
};
