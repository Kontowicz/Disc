#pragma once
#include <string>
#include <array>
#include "Inode.h"
#include "directory.h"
#include <vector>

class Disc
{
public:
	Disc();
	void createFile(char name); 
	void appendFile(char name, std::string data);
	void deleteFile(char name);
	void changeFileName(char name, char newName);

	void createDir(std::string name, std::string nameUpDir); // Tworzymy pusty folder
	void addToDir(std::string name, char nameFile);
	void addInodeToDir(std::string nameDir, char nameFile);
	void deleteDir(std::string name);
	void printDisc();
	void printDir();
	void printFreeBlock();

	void printInodeInDir(std::string name);

	void printAllFiles();

	void setDirName(std::string name, std::string newName);
private:
	std::array<bool, 32> freeBlockVec; // 1 jeœli wolny, 0 Zajêty
	std::array<char, 1024> discTable; // Tablica danych
	std::array<inode, 32> InodeTable; // Tablica iWêz³ów
	std::vector<directory> directories; // Wektor katalogów
	short freeBlocks; // Liczba aktualnie wolnych blokwo dyskowych

	short findInode(char name);
	short getFreeBlock();
	short findDirectory(std::string name);
};
