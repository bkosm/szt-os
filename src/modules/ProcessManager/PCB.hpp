#pragma once
#include <iostream>
#include <string>
#include <queue>
#include <memory>
#include <random>
#include <list>

enum class PCBStatus {
	New = 0,
	Waiting = 1,
	Ready = 2,
	Running = 3,
	Terminated = 4
};

class PCB {
public:
	std::string processName;
	int processID;
	PCBStatus status;
	//Shell& shell;
	//===== PROCESSOR CONTROL ======
	uint8_t estimatedTime;
	uint8_t executedInstr;
	//===== INTEPRETER =====
	uint8_t AX, BX, CX, DX;
	uint8_t insnIndex;

	//===== MEMORY CONTROL =====
	//std::vector<PAGE> insnPages;
	//===== FILE CONTROL =====

	//===== LOCK CONTROL =====
	//Absolutna cisza w samym œrodku szalej¹cego ¿ywio³u. 

	PCB(std::string name, int ID, PCBStatus processstatus);
	~PCB();
	void showProcess();
};

class ProcessManager {
	std::vector<PCB> processList;
	std::vector<PCB*> readyQueue;
public:
	PCB& createProcess(std::string name, std::string fileName);
	int getNextPID();

	std::vector<PCB*> getReadyQueue();
};

int randomPID(int min, int max);
void showChosenProcess(PCB process);

/*struct List {
	std::shared_ptr<PCB> head;
	std::shared_ptr<PCB> tail;

	List() {
		head = std::make_shared<PCB>();
		tail = head;
	}

	void add_node(int value) {
		tail->add_next(value);
		tail = tail->next;
	}

	void print() const
	{
		std::cout << std::string(100, '-') << '\n';
		auto current = head;
		while (current != nullptr) {
			std::cout << current->value << '\t';
			current = current->next;
		}
		std::cout << '\n' << std::string(100, '-') << '\n';
	}

	void reverse() {
		auto current = head;
		std::shared_ptr<Node> next, prev;
		while (current != nullptr) {
			next = current->next;
			current->next = prev;
			prev = current;
			current = next;
		}
		head = prev;
	}

	void insert_after(int after_value, int value) const
	{
		auto current = head;
		while (current != nullptr) {
			if (current->value == after_value) {
				auto temp_next = current->next;
				auto node = create_node(value);
				current->next = node;
				node->next = temp_next;
				break;
			}
			current = current->next;
		}
	}

	void insert_before(int before_value, int value) const
	{
		auto current = head;
		std::shared_ptr<Node> prev;
		while (current != nullptr) {
			if (current->value == before_value) {
				auto node = create_node(value);
				node->next = current;
				prev->next = node;
			}
			prev = current;
			current = current->next;
		}
	}

	std::shared_ptr<Node> create_node(int value) const
	{
		return std::make_shared<Node>(value);
	}
};*/
