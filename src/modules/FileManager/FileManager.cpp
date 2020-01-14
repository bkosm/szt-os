#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>  
#include "FileManager.h"
#include "../../Shell.hpp"
#include "../../SztosException.hpp"

int FileManager::searchFreeBlock() {

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
	throw SztosException("Plik przekroczyl maksymalna wielkosc.");
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

	closeFile(name);
	mainCatalog.erase(mainCatalog.begin() + ind);
	return 0;
}

int FileManager::openFile(std::string name, std::shared_ptr<PCB> pcb) {
	int ind = searchFileId(name);

	//if (!mainCatalog[ind].lock.aquire() and pcb != nullptr)
	//{
	//	pcb->changeStatus(PCBStatus::Waiting);
	//	mainCatalog[ind].lock.getProcessQueue().push_back(pcb);
	//	return -1;
	//}

	for (int i = 0; i < openFiles.size(); i++) {
		if (openFiles[i] == -1 && openFiles[i]) {

			openFiles[i] = ind;
			return 0;
		}
	}
	return 0;
}

int FileManager::closeFile(std::string name) {
	int ind = searchFileId(name);

	/*if (mainCatalog[ind].lock.unlock())
	{
		auto& lockQueue = mainCatalog[ind].lock.getProcessQueue();
		if (!lockQueue.empty())
		{
			auto pcb = lockQueue.front();
			pcb->changeStatus(PCBStatus::Ready);
		}*/

		for (int i = 0; i < openFiles.size(); i++) {
			if (openFiles[i] == ind) {
				openFiles[i] = -1;
				return 0;
			}
		}
	//}
	return -1;
}

int FileManager::writeToFile(std::string name, std::string data) {
	for (auto fileIndex : openFiles) {

		if (mainCatalog[fileIndex].name == name) {

			mainCatalog[fileIndex].size += data.size();

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
			mainCatalog[i].name = newName;
			return 0;
		}
		return 0;
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

	ss << " |       Pojemnosc dysku : " << DISK_CAPACITY << " B\n";
	ss << " |         Rozmiar bloku : " << static_cast<int>(BLOCK_SIZE) << " B\n";
	ss << " |    Maks rozmiar pliku : " << BLOCK_SIZE * BLOCK_SIZE << " B\n";
	ss << " |     Maks ilosc plikow : " << static_cast<int>(DISK_CAPACITY / BLOCK_SIZE) << " Plikow\n";

	return ss.str();
}

std::string FileManager::displayFileInfo(const std::string& name) {
	std::stringstream ss;

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

FileManager::FileManager(Shell* shell) : shell(shell)
{
	openFiles = { -1 };
	disk = { 0 };
}
