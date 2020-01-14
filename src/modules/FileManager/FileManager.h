#pragma once
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "../LockManager/Lock.hpp"


class Shell;

constexpr uint8_t BLOCK_SIZE = 16;
constexpr uint8_t FILE_LIMIT = 16;
constexpr unsigned short DISK_CAPACITY = 1024;

struct File {
	std::string name;
	int indexBlockNumber;
	int size = 0;
	int readPointer;
	int writePointer;
	int byteReadPtr = 0;
	Lock lock;

	File() {
		this->readPointer = 0;
		this->writePointer = 0;
	}

	File(std::string name) {
		this->name = name;
		this->readPointer = 0;
		this->writePointer = 0;
	}
};

class FileManager {
public:
	FileManager(Shell* shell);

	int createFile(std::string name);
	int deleteFile(std::string name);
	int openFile(std::string name, std::shared_ptr<PCB> pcb);
	int closeFile(std::string name, std::shared_ptr<PCB> pcb);
	int writeToFile(std::string name, std::string data);
	uint8_t readFileByte(std::string name);
	int sendFileByte(std::string name, int howMuch);
	int renameFile(std::string name, std::string newName);
	int longFile(std::string name);
	std::vector<File> &getFiles();

	static std::string displayFileSystemParams();
	std::string readFileAll(std::string name);
	std::string displayFileInfo(const std::string& name);
	std::string displayDiskContentChar();
	std::string fileList() const;
	std::string fileLockQueue(std::string name) const;
	std::string showBlock(int index) const;

private:
	Shell* shell;

	std::array<char, DISK_CAPACITY> disk{};
	std::vector<File> mainCatalog;
	std::array<int, DISK_CAPACITY / BLOCK_SIZE> openFiles{};
	std::array<bool, DISK_CAPACITY / BLOCK_SIZE> freeIndexes{};

	File* getFileByName(std::string const& name);
	
	bool isNameUsed(std::string name);
	int searchFreeBlock();
	int searchFileId(std::string name);
	int searchIndexBlock(int fileIndex);
};
