#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>  
#include <algorithm>
#include "FileManager.h"
#include "../../Shell.hpp"
#include "../../SztosException.hpp"

std::vector<File>& FileManager::getFiles() {
	return mainCatalog;
}

void FileManager::isFileNameExist(std::string name)
{
	bool exist = false;
	for (auto file : getFiles()) {

		if (file.name == name) {
			exist = true;
		}

	}
	if (exist == false) {
		throw SztosException("The file with the given name does not exist.");
	}
}


int FileManager::searchFreeBlock() {
	if (std::all_of(freeIndexes.begin(), freeIndexes.end(), [](bool isFree) { return isFree; }))
	{
		throw SztosException("Maximum file size exceeded.");
	}
	for (int i = 0; i < freeIndexes.size(); i++) {
		if (freeIndexes[i] == false) {
			freeIndexes[i] = true;
			return i;
		}
	}
	return -1;
}

int FileManager::searchIndexBlock(int fileIndex) {
	const auto& fileIndexBlock = mainCatalog[fileIndex].indexBlockNumber;

	for (int j = 0; j < 16; j++) {
		auto& indexBlockElem = disk[fileIndexBlock * BLOCK_SIZE + j];

		if (indexBlockElem != 0)
		{
			for (int b = 0; b < 16; b++) {
				if (disk[indexBlockElem * BLOCK_SIZE + b] == 0) {
					return indexBlockElem;
				}
			}
		}
		else {
			indexBlockElem = searchFreeBlock();
			return indexBlockElem;
		}
	}
	throw SztosException("Maximum file size exceeded.");
}

File* FileManager::getFileByName(std::string const& name)
{
	const auto file = std::find_if(std::begin(mainCatalog), std::end(mainCatalog), [&name](File const& f)
		{
			return f.name == name;
		});

	if (file == std::end(mainCatalog))
	{
		throw SztosException("Plik o podanej nazwie nie istnieje");
	}

	return &*file;
}

bool FileManager::isNameUsed(std::string name) {
	for (int i = 0; i < mainCatalog.size(); i++) {
		if (mainCatalog[i].name == name) return true;
	}

	return false;
}

int FileManager::searchFileId(std::string name) {
	for (int i = 0; i < mainCatalog.size(); i++) {
		if (mainCatalog[i].name == name) return i;
	}
	return -1;
}

int FileManager::createFile(std::string name) {

	//Czesc sprawdzajaca
	if (isNameUsed(name)) return -1;

	if (name.empty()) return -1;

	if (mainCatalog.size() >= FILE_LIMIT) return -1;
	//Czesc dzialajaca
	File newFile(name);
	newFile.lock = shell->getLockManager().createLock();
	newFile.indexBlockNumber = searchFreeBlock();
	mainCatalog.push_back(newFile);

	return 0;
}

int FileManager::deleteFile(std::string name) {
	int ind = searchFileId(name);

	for (auto i = 0; i < 16; i++)
	{
		auto blockIndex = disk[mainCatalog[ind].indexBlockNumber * BLOCK_SIZE + i];
		freeIndexes[blockIndex] = false;
		if (blockIndex == 0) break;
	}
	freeIndexes[mainCatalog[ind].indexBlockNumber] = false;

	for (int index = 0; index < 16; index++) {
		auto& blockIndex = disk[mainCatalog[ind].indexBlockNumber * BLOCK_SIZE + index];

		if (blockIndex == 0) break;

		for (auto i = 0; i < 16; i++) {
			auto& aByte = disk[blockIndex * BLOCK_SIZE + i];
			aByte = 0;
		}
		blockIndex = 0;
	}

	mainCatalog.erase(mainCatalog.begin() + ind);

	for (int i = 0; i < openFiles.size(); i++) {
		if (openFiles[i] == ind) {
			openFiles[i] = -1;
			return 0;
		}
	}

	return 0;
}

int FileManager::openFile(std::string name, std::shared_ptr<PCB> pcb) {
	int ind = searchFileId(name);

	isFileNameExist(name);

	if (!mainCatalog[ind].lock.aquire(pcb))
	{
		if (pcb != nullptr) {
			pcb->changeStatus(PCBStatus::Waiting);
			shell->getProcessManager().deleteProcessFromQueue(pcb->getPID());
		}
		return -1;
	}

	for (int i = 0; i < openFiles.size(); i++) {
		if (openFiles[i] == -1) {

			openFiles[i] = ind;
			return 0;
		}
	}
	return -1;
}

int FileManager::closeFile(std::string name, std::shared_ptr<PCB> pcb) {
	int ind = searchFileId(name);

	isFileNameExist(name);

	if (mainCatalog[ind].lock.unlock(pcb))
	{
		if (pcb != nullptr) {
			auto queue = mainCatalog[ind].lock.getProcessQueue();

			if (!queue.empty())
			{
				queue.front()->changeStatus(PCBStatus::Ready);
				shell->getProcessManager().addProcessToQueue(queue.front());
			}
		}

		for (int i = 0; i < openFiles.size(); i++) {
			if (openFiles[i] == ind) {
				openFiles[i] = -1;
				return 0;
			}
		}
	}
	return -2;
}

int FileManager::writeToFile(std::string name, std::string data) {
	for (auto fileIndex : openFiles) {

		if (mainCatalog[fileIndex].name == name) {

			if (mainCatalog[fileIndex].size + data.size() > 256)
			{
				mainCatalog[fileIndex].size = 256;
			}
			else
			{
				mainCatalog[fileIndex].size += data.size();
			}


			for (auto letter : data)
			{
				auto blockToWrite = searchIndexBlock(fileIndex);

				disk[blockToWrite * BLOCK_SIZE + mainCatalog[fileIndex].writePointer] = letter;
				mainCatalog[fileIndex].writePointer++;
				if (mainCatalog[fileIndex].writePointer > 15)
				{
					mainCatalog[fileIndex].writePointer = 0;
				}
			}
			return 0;

		}
	}
	return 0;
}

int FileManager::sendFileByte(std::string name, int howMuch) {
	int block;
	char result;

	for (auto i : openFiles) {
		if (mainCatalog[i].name == name) {
			mainCatalog[i].readPointer = 0;
			for (int j = mainCatalog[i].readPointer; j < howMuch;) {
				int k = 0;
				if (j % 8 == 0 && j > 1)k++;
				block = disk[mainCatalog[i].indexBlockNumber * BLOCK_SIZE + k] * BLOCK_SIZE;
				result = disk[block + mainCatalog[i].readPointer % BLOCK_SIZE];
				mainCatalog[i].readPointer++;
			}
			return result;
		}
	}
}

uint8_t FileManager::readFileByte(std::string name) {
	auto file = getFileByName(name);

	if (file->byteReadPtr >= file->size)
	{
		file->byteReadPtr = 0;
		return 0;
	}

	const int blockNum = file->byteReadPtr / BLOCK_SIZE;
	const int blockIndex = file->byteReadPtr % BLOCK_SIZE;
	const int offset = disk[file->indexBlockNumber * BLOCK_SIZE + blockNum];

	file->byteReadPtr++;
	return disk[offset * BLOCK_SIZE + blockIndex];
}

std::string FileManager::readFileAll(std::string name) {
	for (auto i : openFiles) {
		if (mainCatalog[i].name == name) {
			std::string result;
			for (auto index = 0; index < 16; index++) {
				auto blockIndex = disk[mainCatalog[i].indexBlockNumber * BLOCK_SIZE + index];
				for (auto i = 0; i < 16; i++) {
					if (blockIndex != 0) {
						auto aByte = disk[blockIndex * BLOCK_SIZE + i];
						if (aByte == 0) return result;

						result += disk[blockIndex * BLOCK_SIZE + i];
					}
				}
			}
			return result;
		}
	}
}

int FileManager::renameFile(std::string name, std::string newName) {

	for (int i = 0; i < mainCatalog.size(); i++) {
		if (mainCatalog[i].name == name) {
			for (int j = 0; j < mainCatalog.size(); j++)
			{
				if (mainCatalog[j].name == newName)
				{
					throw SztosException("A file with the given name already exists.");
				}
			}
			mainCatalog[i].name = newName;
			return 0;
		}
	}
	return 0;
}

int FileManager::longFile(std::string name) {

	for (int i = 0; i < mainCatalog.size(); i++) {
		if (mainCatalog[i].name == name) return mainCatalog[i].size;
	}
	return -1;
}

std::string FileManager::displayFileSystemParams() {
	std::stringstream ss;

	ss << "Disk capacity:\t" << DISK_CAPACITY << " B\n";
	ss << "Block size:\t" << static_cast<int>(BLOCK_SIZE) << " B\n";
	ss << "Max file size:\t" << BLOCK_SIZE * BLOCK_SIZE << " B\n";
	ss << "Max file number:\t" << static_cast<int>(DISK_CAPACITY / BLOCK_SIZE) << "\n";

	return ss.str();
}

std::string FileManager::displayFileInfo(const std::string& name) {
	std::stringstream ss;

	isFileNameExist(name);

	ss << "File name:\t" << name << "\n";
	int i = searchFileId(name);
	ss << "File size:\t" << mainCatalog[i].size << " [B]\n";
	ss << "Index block:\t" << mainCatalog[i].indexBlockNumber << " \n";

	return ss.str();
}

std::string FileManager::displayDiskContentChar() {
	std::ostringstream output;

	unsigned frameIndex = 0, pageNumber = 0, column = 0;

	output << "DISK CONTENT:" << std::endl;
	for (auto ch : disk)
	{
		if (ch == '\t' or ch == '\n') ch = ' ';

		if (column == 2) {
			if (frameIndex == 0)
			{
				output << std::setfill('0') << std::setw(2) << pageNumber << ": {" << ch;
				++pageNumber;
			}
			else if (frameIndex == 15)
			{
				output << ch << "}" << std::endl;
				frameIndex = -1;
				column = 0;
			}
			else
			{
				output << ch;
			}
			++frameIndex;
		}
		else {
			if (frameIndex == 0)
			{
				output << std::setfill('0') << std::setw(2) << pageNumber << ": {" << ch;
				++pageNumber;
			}
			else if (frameIndex == 15)
			{
				output << ch << "} ";
				frameIndex = -1;
				++column;
			}
			else
			{
				output << ch;
			}
			++frameIndex;
		}
	}

	return output.str();
}

std::string FileManager::fileList() const
{
	std::ostringstream ss;
	int counter = 1;

	for (const auto& file : mainCatalog)
	{
		ss << std::setfill('0') << std::setw(3) << counter
			<< ": " << file.name << "\t[size: " << file.size << "B]"
			<< "\tLock State: " << std::boolalpha << file.lock.getState() << std::endl;
		counter++;
	}

	return ss.str();
}

std::string FileManager::fileLockQueue(std::string name) const
{
	std::ostringstream ss;
	int counter = 1;

	for (int i = 0; i < mainCatalog.size(); i++) {
		if (mainCatalog[i].name == name)
		{
			ss << mainCatalog[i].lock.getProcessQueueString() << std::endl;
			break;
		}
	}

	return ss.str();
}

std::string FileManager::showBlock(int index) const
{
	std::ostringstream ss;

	ss << "BLOCK " << index << " CONTAINS: " << std::endl;
	for (int i = 0; i < 16; i++)
	{
		ss << "[" << std::setfill('0') << std::setw(2) << i << "] " << std::setfill(' ') << std::setw(4) << int(disk[index * BLOCK_SIZE + i]) << "\t" << disk[index * BLOCK_SIZE + i] << std::endl;
	}

	return ss.str();
}

FileManager::FileManager(Shell* shell) : shell(shell)
{
	openFiles.fill(-1);
	disk.fill(0);
}
