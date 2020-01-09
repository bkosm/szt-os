#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>  
#include "FileManager.h"

FileManager fm;

//Aliasy
using u_int = unsigned int;
using u_short_int = unsigned short int;
using u_char = unsigned char;

int FileManager::searchFreeBlock() {

	for (int i = 0; i < freeIndexes.size(); i++) {
		if (freeIndexes[i] == false) {
			freeIndexes[i] = true;
			return i;
		}
	}
	return -1;
}

int FileManager::searchIndexBlock(int i) {
	int block;
	int block2;
	for (int j = 0; j < 8; j++) {
		if (disk[mainCatalog[i].indexBlockNumber * BLOCK_SIZE + j] == 0) {
			block2 = searchFreeBlock();
			indexBlockFillZero(block2);
		}

		if (disk[mainCatalog[i].indexBlockNumber * BLOCK_SIZE + j] * BLOCK_SIZE == 0) {
			block = mainCatalog[i].indexBlockNumber * BLOCK_SIZE + j;
			return block;
		}
	}

}

bool FileManager::isNameUsed(std::string name) {
	for (int i = 0; i < mainCatalog.size(); i++) {
		if (mainCatalog[i].name == name) return true;
	}

	return false;
}

int FileManager::indexBlockFillZero(int ind) {
	for (int i = 0; i < 8; i++) {
		disk[ind * BLOCK_SIZE + i] = 0;
	}

	return 0;
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
	newFile.indexBlockNumber = searchFreeBlock();
	indexBlockFillZero(newFile.indexBlockNumber);
	mainCatalog.push_back(newFile);

	return 0;
}

int FileManager::deleteFile(std::string name) {
	int ind = searchFileId(name);
	closeFile(name);
	mainCatalog.erase(mainCatalog.begin() + ind);
	return 0;
}

int FileManager::openFile(std::string name) {
	int ind = searchFileId(name);
	for (int i = 0; i < openFiles.size(); i++) {
		if (openFiles[i] == -1) {
			openFiles[i] = ind;
			return 0;
		}
	}
	return 0;
}

int FileManager::closeFile(std::string name) {
	int ind = searchFileId(name);
	for (int i = 0; i < openFiles.size(); i++) {
		if (openFiles[i] == ind) {
			openFiles[i] = -1;
			return 0;
		}
	}
	return 0;
}

int FileManager::writeToFile(std::string name, std::string data) {
	int block;

	for (auto i : openFiles) {

		if (mainCatalog[i].name == name) {
			mainCatalog[i].size += data.size();
			for (int j = 0; j < data.size(); j++) {

				block = searchIndexBlock(i);
				disk[block * BLOCK_SIZE + mainCatalog[i].writePointer] = data[j];
				mainCatalog[i].writePointer++;
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

int FileManager::readFileByte(std::string name, int howMuch) {
	for (auto i : openFiles) {

		if (mainCatalog[i].name == name) {
			if (mainCatalog[i].readPointer == howMuch) {
				mainCatalog[i].readPointer = 0;
				return 0;
			}

			for (int j = mainCatalog[i].readPointer; j < howMuch;) {
				readFileByte(name, howMuch);
			}
		}

		return 0;
	}
}

std::string FileManager::readFileAll(std::string name) {
	int block;
	std::string result;

	for (auto i : openFiles) {
		if (mainCatalog[i].name == name) {
			mainCatalog[i].readPointer = 0;

			for (int j = 0; j < mainCatalog[i].size; j++) {
				int k = 0;
				if (j % 8 == 0 && j > 1)k++;
				block = disk[mainCatalog[i].indexBlockNumber * BLOCK_SIZE + k] * BLOCK_SIZE;
				result += disk[block + mainCatalog[i].readPointer % BLOCK_SIZE];
				mainCatalog[i].readPointer++;
			}
			mainCatalog[i].readPointer = 0;
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

	ss << " |       Nazwa Pliku : " << name << "\n";
	int i = searchFileId(name);
	ss << " |         Rozmiar pliku : " << mainCatalog[i].size << " B\n";
	ss << " |     Numer bloku indeksowego : " << mainCatalog[i].indexBlockNumber << " \n";

	return ss.str();
}

std::string FileManager::displayDiskContentChar() {
	std::stringstream ss;

	for (u_int i = 0; i < DISK_CAPACITY / BLOCK_SIZE; i++) {
		ss << std::setfill('0') << std::setw(2) << i << ".  ";
		for (u_int j = 0; j < BLOCK_SIZE; j++) {
			if (disk[i * BLOCK_SIZE + j] >= 0 && disk[i * BLOCK_SIZE + j] <= 16) { ss << "."; }
			else { ss << disk[i * BLOCK_SIZE + j]; }
		}
		if (i % 2 == 1) { ss << '\n'; }
		else { ss << "  "; }
	}
	ss << '\n';

	return ss.str();
}