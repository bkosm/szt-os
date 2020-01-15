#include "Interpreter.hpp"
#include <unordered_map>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <memory>
#include "../../Shell.hpp"
#include "../../SztosException.hpp"
#include "../../modules/ProcessManager/PCB.hpp"
#include <iostream>

// instantiate pair <string, method ptr> for each instruction available
#define INSN(NAME) { #NAME, &Interpreter::insn##NAME }

// simplify duplicative code
// can't make function of this because of 'process' parameter
#define LOAD(ARG) std::string ARG = readNextParam(process)
#define GET(DEST) getValue(process, DEST)
#define SET(DEST, VAL) setValue(process, DEST, VAL)

// simplify certain lambda creation
#define MATH_LAMBDA(EXP) [](uint8_t a, uint8_t b) -> uint8_t { return EXP; }
#define JUMP_LAMBDA(EXP) [](uint8_t a, uint8_t b) -> bool { return EXP; }

Interpreter::Interpreter(Shell *shell) : insnMap {
	INSN(ADD),	INSN(SUB),	INSN(MUL),	INSN(DIV),	INSN(INC),	INSN(DSC),
	INSN(JP),	INSN(JZ),	INSN(JNZ),
	INSN(JE),	INSN(JNE),	INSN(JA),	INSN(JAE),	INSN(JB),	INSN(JBE),
	INSN(LO),	INSN(CP),
	INSN(FCR),	INSN(FO),	INSN(FC),	INSN(FR),	INSN(FW)
}, shell(shell), lastInsn()
{
}
#undef INSN

Interpreter::~Interpreter() {}

std::string Interpreter::readNextParam(std::shared_ptr<PCB> process) {
	char c;
	bool loadsFileName = false;
	for (std::string buffer = "";; buffer += c) {
		try {
			c = static_cast<char>(shell->getMemoryManager().getByte(*process, process->insnIndex));
		} catch (SztosException &e) {
			throw e;
		}

		++process->insnIndex;

		if (c == '\"') loadsFileName = !loadsFileName;
		if (!loadsFileName && c == ' ') {
			lastInsn += ' ' + buffer;
			return buffer;
		}
	}
}

void Interpreter::escapeQuote(std::string &s)
{
	if (s.length() < 2) {
		throw SztosException("Argument nie jest w cudzyslowiach (" + s + ").");
	}
	
	if (s.front() != '\"' || s.back() != '\"')
	{
		throw SztosException("Nie udalo sie usunac cudzyslowiow z argumentu (" + s + ").");
	}
	
	s = s.substr(1, s.length() - 2);
}

void Interpreter::handleInsn(std::shared_ptr<PCB> process) {
	uint8_t prevInsnIndex = process->insnIndex;
	lastInsn = "";

	char c;
	for (std::string buffer = "";;) {
		try {
			c = static_cast<char>(shell->getMemoryManager().getByte(*process, process->insnIndex));
		} catch (SztosException &e) {
			process->insnIndex = prevInsnIndex;
			process->changeStatus(PCBStatus::Error);
			throw e;
		}
		
		++process->insnIndex;

		if (c == ' ') {
			void (Interpreter::*insn)(std::shared_ptr<PCB>) = nullptr;

			try {
				insn = insnMap.at(buffer);
			} catch (std::out_of_range &e) {
				process->changeStatus(PCBStatus::Error);
				process->insnIndex = prevInsnIndex;
				throw SztosException("Nieznana instrukcja: " + buffer);
			}

			lastInsn = buffer;
			try {
				(this->*insn)(process);
				++process->insnCounter;
			} catch (SztosException &e) {
				process->changeStatus(PCBStatus::Error);
				process->insnIndex = prevInsnIndex;
				throw e;
			}
			return;
		}

		buffer += c;
		if (buffer == "HLT") {
			process->changeStatus(PCBStatus::Terminated);
			lastInsn = "HLT";
			return;
		}
	}
}

std::string Interpreter::getLastInsn() const
{
	return lastInsn;
}

std::string Interpreter::loadDummyProgram() {
	return "JP 0 ";
}

std::string Interpreter::loadProgram(const std::string name) {
	std::ifstream in("programs/" + name);
	if (!in.is_open()) {
		throw SztosException("Nie ma takiego pliku.");
	}

	std::string prog = "";
	std::string temp;
	int mode = 0;
	while (std::getline(in, temp)) {
		if (temp == ".text") {
			mode = 0;
			continue;
		}
		else if (temp == ".data") {
			mode = 1;
			continue;
		}

		if (mode == 0) {
			prog += temp;
			prog += ' ';
		}
		else if (mode == 1) {
			try {
				prog += static_cast<char>(std::stoul(temp));
			} catch (std::exception &e) {
				throw SztosException("Blad konwersji liczb w programie.");
			}
		}
	}

	return prog;
}

uint8_t Interpreter::getValue(std::shared_ptr<PCB> process, std::string dest) {
	if (dest.length() == 0) throw SztosException("Parametr instrukcji jest pusty.");

	bool isAddr;
	if (dest.front() == '[' && dest.back() == ']') {
		dest = dest.substr(1, dest.length() - 2);
		isAddr = true;
	}
	else isAddr = false;

	uint8_t value;

	try {
		if (dest == "AX") { value = process->AX; }
		else if (dest == "BX") { value = process->BX; }
		else if (dest == "CX") { value = process->CX; }
		else if (dest == "DX") { value = process->DX; }
		else value = static_cast<uint8_t>(std::stoul(dest));
	} catch (std::exception &e) {
		throw SztosException("Nie udalo sie przekonwertowac parametru instrukcji na liczbe (w poleceniu: \"" + lastInsn + "\").");
	}

	if (isAddr) {
		try {
			value = shell->getMemoryManager().getByte(*process, value);
		} catch (SztosException &e) {
			throw e;
		}
		
	}

	return value;
}

void Interpreter::setValue(std::shared_ptr<PCB> process, std::string dest, uint8_t value) {
	if (dest.length() == 0) throw SztosException("Parametr instrukcji jest pusty.");

	if (dest.front() == '[' && dest.back() == ']') {
		dest = dest.substr(1, dest.length() - 2);

		uint8_t target;
		try {
			if (dest == "AX") { target = process->AX; }
			else if (dest == "BX") { target = process->BX; }
			else if (dest == "CX") { target = process->CX; }
			else if (dest == "DX") { target = process->DX; }
			else target = static_cast<uint8_t>(std::stoul(dest));
		} catch (std::exception &e) {
			throw SztosException("Nie udalo sie przekonwertowac parametru instrukcji na liczbe (w poleceniu: \"" + lastInsn + "\").");
		}

		try {
			shell->getMemoryManager().setByte(*process, target, value);
		} catch (SztosException &e) {
			throw e;
		}
	}
	else if (dest == "AX") { process->AX = value; }
	else if (dest == "BX") { process->BX = value; }
	else if (dest == "CX") { process->CX = value; }
	else if (dest == "DX") { process->DX = value; }
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::handleArithm(std::shared_ptr<PCB> process, uint8_t(*op)(uint8_t, uint8_t)) {
	LOAD(param1); LOAD(param2);
	SET(param1, op(GET(param1), GET(param2)));
}

void Interpreter::insnADD(std::shared_ptr<PCB> process) { handleArithm(process, MATH_LAMBDA(a + b)); }
void Interpreter::insnSUB(std::shared_ptr<PCB> process) { handleArithm(process, MATH_LAMBDA(a - b)); }
void Interpreter::insnMUL(std::shared_ptr<PCB> process) { handleArithm(process, MATH_LAMBDA(a * b)); }
void Interpreter::insnDIV(std::shared_ptr<PCB> process) { handleArithm(process, MATH_LAMBDA(a / b)); }
void Interpreter::insnINC(std::shared_ptr<PCB> process) { LOAD(param1); SET(param1, GET(param1) + 1); }
void Interpreter::insnDSC(std::shared_ptr<PCB> process) { LOAD(param1); SET(param1, GET(param1) - 1); }

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::handleJump(std::shared_ptr<PCB> process, bool(*op)(uint8_t, uint8_t)) {
	LOAD(param1); LOAD(param2); LOAD(param3);
	if (op(GET(param1), GET(param2))) process->insnIndex = GET(param3);
}

void Interpreter::insnJP(std::shared_ptr<PCB> process) { LOAD(param1); process->insnIndex = GET(param1); }

void Interpreter::insnJZ(std::shared_ptr<PCB> process) {
	LOAD(param1); LOAD(param2);
	if (GET(param1) == 0) process->insnIndex = GET(param2);
}

void Interpreter::insnJNZ(std::shared_ptr<PCB> process) {
	LOAD(param1); LOAD(param2);
	if (GET(param1) != 0) process->insnIndex = GET(param2);
}

void Interpreter::insnJE(std::shared_ptr<PCB> process)	{ handleJump(process, JUMP_LAMBDA(a == b)); }
void Interpreter::insnJNE(std::shared_ptr<PCB> process)	{ handleJump(process, JUMP_LAMBDA(a != b)); }
void Interpreter::insnJA(std::shared_ptr<PCB> process)	{ handleJump(process, JUMP_LAMBDA(a >  b)); }
void Interpreter::insnJAE(std::shared_ptr<PCB> process)	{ handleJump(process, JUMP_LAMBDA(a >= b)); }
void Interpreter::insnJB(std::shared_ptr<PCB> process)	{ handleJump(process, JUMP_LAMBDA(a <  b)); }
void Interpreter::insnJBE(std::shared_ptr<PCB> process)	{ handleJump(process, JUMP_LAMBDA(a <= b)); }

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::insnLO(std::shared_ptr<PCB> process) {
	LOAD(param1); LOAD(param2);
	SET(param1, GET(param2));
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::insnCP(std::shared_ptr<PCB> process) {
	LOAD(param1); LOAD(param2);

	try
	{
		escapeQuote(param1); escapeQuote(param2);
		shell->getProcessManager().createProcess(param1, param2);
	}
	catch (SztosException & e)
	{
		throw e;
	}
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::insnFCR(std::shared_ptr<PCB> process) {
	LOAD(param1);
	try
	{
		escapeQuote(param1);
		shell->getFileManager().createFile(param1);
	}
	catch (SztosException & e)
	{
		throw e;
	}
}

void Interpreter::insnFO(std::shared_ptr<PCB> process) {
	LOAD(param1);
	try
	{
		escapeQuote(param1);
		shell->getFileManager().openFile(param1, process);
	}
	catch (SztosException & e)
	{
		throw e;
	}
}

void Interpreter::insnFC(std::shared_ptr<PCB> process) {
	LOAD(param1);
	try
	{
		escapeQuote(param1);
		shell->getFileManager().closeFile(param1,process);
	}
	catch (SztosException & e)
	{
		throw e;
	}
}

void Interpreter::insnFR(std::shared_ptr<PCB> process) {
	LOAD(param1); LOAD(param2);

	try
	{
		escapeQuote(param2);
		uint8_t value = shell->getFileManager().readFileByte(param2);
		SET(param1, value);
	}
	catch (SztosException & e)
	{
		throw e;
	}
}

void Interpreter::insnFW(std::shared_ptr<PCB> process) {
	LOAD(param1); LOAD(param2);

	uint8_t value = GET(param1);
	try
	{
		escapeQuote(param2);
		
		std::string temp = "";
		temp += static_cast<char>(value);

		shell->getFileManager().writeToFile(param2, temp);
	}
	catch (SztosException & e)
	{
		throw e;
	}
}
