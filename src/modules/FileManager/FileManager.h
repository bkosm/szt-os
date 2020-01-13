#pragma once
#include <string>
#include <vector>
#include <array>

class Shell;

class FileManager {
public:
	using u_int = unsigned int;
	using u_short_int = unsigned short int;

	FileManager(Shell* shell);

	static const uint8_t BLOCK_SIZE = 16;
	static const uint8_t FILE_LIMIT = 16;
	static const u_short_int DISK_CAPACITY = 1024;

	struct File {
		std::string name;
		int indexBlockNumber;
		int size = 0;
		int readPointer;
		int writePointer;

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
	int indexBlockFillZero(int ind);
	int searchIndexBlock(int fileIndex);
};
