#pragma once
#include <unordered_map>
#include <string>
#include <cstdint>

// dummy PCB for insnMap
struct PCB {
	uint8_t AX, BX, CX, DX;
	uint8_t IC;
	bool state;
};

// dummy Shell for nothing yet
struct Shell {};

#define DECL(NAME) void insn##NAME##(PCB &process)
class Interpreter {
	std::unordered_map<std::string, void(Interpreter::*)(PCB &)> insnMap;
	Shell &shell;

	// declare method for each instruction available
	DECL(ADD);	DECL(SUB);	DECL(MUL);	DECL(DIV);	DECL(INC);	DECL(DSC);
	DECL(JP);	DECL(JZ);	DECL(JNZ);
	DECL(JE);	DECL(JNE);	DECL(JA);	DECL(JAE);	DECL(JB);	DECL(JBE);
	DECL(LO);	DECL(CP);	DECL(FO);	DECL(FC);	DECL(FR);	DECL(FW);

	std::string readNextParam(PCB &process);

	uint8_t getValue(PCB &process, std::string dest);
	void setValue(PCB &process, std::string dest, uint8_t value);

	void handleArithm(PCB &process, uint8_t(*op)(uint8_t, uint8_t));
	void handleJump(PCB &process, bool (*op)(uint8_t, uint8_t));

public:
	Interpreter(Shell &shell);
	~Interpreter();

	void handleInsn(PCB &process);
};
#undef DECL
