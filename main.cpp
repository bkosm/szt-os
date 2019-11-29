#include <iostream>
#include <cstdint>
#include "Interpreter.hpp"

int main() {
	Shell shell;
	Interpreter interpreter(shell);

	// test to fully compile class
	uint8_t x;
	std::cin >> x;

	PCB pcb = { x, 0, 0, 0, 0, 1 };
	interpreter.handleInsn(pcb);

	return 0;
}