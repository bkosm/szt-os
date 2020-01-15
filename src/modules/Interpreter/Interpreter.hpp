#pragma once
#include <unordered_map>
#include <string>
#include <cstdint>
#include <memory>
#include "../../modules/ProcessManager/PCB.hpp"

class Shell;

class Interpreter {
	std::unordered_map<std::string, void(Interpreter::*)(std::shared_ptr<PCB>)> insnMap;
	Shell *shell;

	std::string lastInsn;

	void handleArithm(std::shared_ptr<PCB> process, uint8_t(*op)(uint8_t, uint8_t));
	void insnADD(std::shared_ptr<PCB> process); void insnSUB(std::shared_ptr<PCB> process); void insnMUL(std::shared_ptr<PCB> process);
	void insnDIV(std::shared_ptr<PCB> process); void insnINC(std::shared_ptr<PCB> process); void insnDSC(std::shared_ptr<PCB> process);

	void handleJump(std::shared_ptr<PCB> process, bool(*op)(uint8_t, uint8_t));
	void insnJP(std::shared_ptr<PCB> process);
	void insnJZ(std::shared_ptr<PCB> process); void insnJNZ(std::shared_ptr<PCB> process);
	void insnJE(std::shared_ptr<PCB> process); void insnJNE(std::shared_ptr<PCB> process);
	void insnJA(std::shared_ptr<PCB> process); void insnJAE(std::shared_ptr<PCB> process);
	void insnJB(std::shared_ptr<PCB> process); void insnJBE(std::shared_ptr<PCB> process);

	void insnLO(std::shared_ptr<PCB> process);

	void insnCP(std::shared_ptr<PCB> process);

	void insnFCR(std::shared_ptr<PCB> process);
	void insnFO(std::shared_ptr<PCB> process); void insnFC(std::shared_ptr<PCB> process);
	void insnFR(std::shared_ptr<PCB> process); void insnFW(std::shared_ptr<PCB> process);

	std::string readNextParam(std::shared_ptr<PCB> process);
	void escapeQuote(std::string &s);

	uint8_t getValue(std::shared_ptr<PCB> process, std::string dest);
	void setValue(std::shared_ptr<PCB> process, std::string dest, uint8_t value);

public:
	Interpreter(Shell *shell);
	~Interpreter();

	void handleInsn(std::shared_ptr<PCB> process);
	std::string getLastInsn() const;
	std::string loadDummyProgram();
	std::string loadProgram(const std::string name);
};
