#pragma once
#include <string>
#include <vector>
#include <array>
#include "../LockManager/Lock.hpp"

class Shell;

constexpr uint8_t BLOCK_SIZE = 16;
constexpr uint8_t FILE_LIMIT = 16;
constexpr unsigned short DISK_CAPACITY = 1024;

class FileManager {
public:
	FileManager(Shell* shell);

	struct File {
		std::string name;
		int indexBlockNumber;
		int size = 0;
		int readPointer;
		int writePointer;
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

	int createFile(std::string name);
	int deleteFile(std::string name);
	int openFile(std::string name);
	int closeFile(std::string name);
	int writeToFile(std::string name, std::string data);
	int readFileByte(std::string name, int howMuch);
	int sendFileByte(std::string name, int howMuch);
	int renameFile(std::string name, std::string newName);
	int longFile(std::string name);

	static std::string displayFileSystemParams();
	std::string readFileAll(std::string name);
	std::string displayFileInfo(const std::string& name);
	std::string displayDiskContentChar();

private:
	Shell* shell;

	std::array<char, DISK_CAPACITY> disk{};
	std::vector<File> mainCatalog;
	std::array<int, DISK_CAPACITY / BLOCK_SIZE> openFiles{};
	std::array<bool, DISK_CAPACITY / BLOCK_SIZE> freeIndexes{};

	bool isNameUsed(std::string name);
	int searchFreeBlock();
	int searchFileId(std::string name);
	int searchIndexBlock(int fileIndex);
};
