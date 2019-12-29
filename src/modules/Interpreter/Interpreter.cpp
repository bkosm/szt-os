#include "Interpreter.hpp"
#include <unordered_map>
#include <string>
#include <cstdint>
#include <stdexcept>
#include "../../modules/ProcessManager/PCB.hpp"

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
	INSN(LO),	INSN(CP),	INSN(FO),	INSN(FC),	INSN(FR),	INSN(FW)
}, shell(shell)
{
}
#undef INSN

Interpreter::~Interpreter() {}

std::string Interpreter::readNextParam(PCB &process) {
	char c;
	bool loadsFileName = false;
	for (std::string buffer = "";; buffer += c) {
		c = ' ';// TODO read byte from RAM using IC
		++process.insnIndex;

		if (c == '\"') loadsFileName = !loadsFileName;
		if (!loadsFileName && c == ' ') return buffer;
	}
}

void Interpreter::handleInsn(PCB &process) {
	char c;
	
	for (std::string buffer = "";;) {
		if (process.AX == 0) break;// dummy break (just to fully compile class)

		c = ' ';// TODO read byte from RAM using IC
		++process.insnIndex;

		if (c == ' ') {
			try {
				(this->*insnMap.at(buffer))(process);
			} catch (std::out_of_range) {
				// TODO error, not known instruction
			}
			return;
		}

		buffer += c;
		if (buffer == "HLT") {
			// TODO terminate process
			return;
		}
	}
}

uint8_t Interpreter::getValue(PCB &process, std::string dest) {
	if (dest.length() == 0) return 0;// TODO handle error

	bool isAddr;
	if (dest.front() == '[' && dest.back() == ']') {
		dest = dest.substr(1, dest.length() - 2);
		isAddr = true;
	}
	else isAddr = false;

	uint8_t value;

	try {
		if (dest == "AX") { value = process.AX; }
		else if (dest == "BX") { value = process.BX; }
		else if (dest == "CX") { value = process.CX; }
		else if (dest == "DX") { value = process.DX; }
		else value = static_cast<uint8_t>(std::stoul(dest));
	} catch (...) {
		value = 0;// TODO handle error
	}

	if (isAddr) {
		value = 0;// TODO read byte from RAM by value
	}

	return value;
}

void Interpreter::setValue(PCB &process, std::string dest, uint8_t value) {
	if (dest.length() == 0) return;// TODO handle error

	if (dest.front() == '[' && dest.back() == ']') {
		dest = dest.substr(1, dest.length() - 2);

		int target;
		try {
			if (dest == "AX") { target = process.AX; }
			else if (dest == "BX") { target = process.BX; }
			else if (dest == "CX") { target = process.CX; }
			else if (dest == "DX") { target = process.DX; }
			else target = static_cast<uint8_t>(std::stoul(dest));
		} catch (...) {
			target = 0;// TODO handle error
		}

		// TODO set byte in RAM using target
	}
	else if (dest == "AX") { process.AX = value; }
	else if (dest == "BX") { process.BX = value; }
	else if (dest == "CX") { process.CX = value; }
	else if (dest == "DX") { process.DX = value; }
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::handleArithm(PCB &process, uint8_t(*op)(uint8_t, uint8_t)) {
	LOAD(param1); LOAD(param2);
	SET(param1, op(GET(param1), GET(param2)));
}

void Interpreter::insnADD(PCB &process) { handleArithm(process, MATH_LAMBDA(a + b)); }
void Interpreter::insnSUB(PCB &process) { handleArithm(process, MATH_LAMBDA(a - b)); }
void Interpreter::insnMUL(PCB &process) { handleArithm(process, MATH_LAMBDA(a * b)); }
void Interpreter::insnDIV(PCB &process) { handleArithm(process, MATH_LAMBDA(a / b)); }
void Interpreter::insnINC(PCB &process) { LOAD(param1); SET(param1, GET(param1) + 1); }
void Interpreter::insnDSC(PCB &process) { LOAD(param1); SET(param1, GET(param1) - 1); }

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::handleJump(PCB &process, bool(*op)(uint8_t, uint8_t)) {
	LOAD(param1); LOAD(param2); LOAD(param3);
	if (op(GET(param1), GET(param2))) process.insnCounter = GET(param3);
}

void Interpreter::insnJP(PCB &process) { LOAD(param1); process.insnCounter = GET(param1); }

void Interpreter::insnJZ(PCB &process) {
	LOAD(param1); LOAD(param2);
	if (GET(param1) == 0) process.insnCounter = GET(param2);
}

void Interpreter::insnJNZ(PCB &process) {
	LOAD(param1); LOAD(param2);
	if (GET(param1) != 0) process.insnCounter = GET(param2);
}

void Interpreter::insnJE(PCB &process)	{ handleJump(process, JUMP_LAMBDA(a == b)); }
void Interpreter::insnJNE(PCB &process)	{ handleJump(process, JUMP_LAMBDA(a != b)); }
void Interpreter::insnJA(PCB &process)	{ handleJump(process, JUMP_LAMBDA(a >  b)); }
void Interpreter::insnJAE(PCB &process)	{ handleJump(process, JUMP_LAMBDA(a >= b)); }
void Interpreter::insnJB(PCB &process)	{ handleJump(process, JUMP_LAMBDA(a <  b)); }
void Interpreter::insnJBE(PCB &process)	{ handleJump(process, JUMP_LAMBDA(a <= b)); }

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::insnLO(PCB &process) {
	LOAD(param1); LOAD(param2);
	SET(param1, GET(param2));
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::insnCP(PCB &process) {
	LOAD(param1); LOAD(param2);
	// TODO create process
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void Interpreter::insnFO(PCB &process) {
	LOAD(param1);
	// TODO open file by FileManager using process and param1
}

void Interpreter::insnFC(PCB &process) {
	LOAD(param1);
	// TODO open file by FileManager using process and param1
}

void Interpreter::insnFR(PCB &process) {
	LOAD(param1); LOAD(param2);

	uint8_t value = 0;// TODO read byte by FileManager using process and param2
	SET(param1, value);
}

void Interpreter::insnFW(PCB &process) {
	LOAD(param1); LOAD(param2);

	uint8_t value = getValue(process, param1);
	// TODO write byte by FileManager using process and param2
}
