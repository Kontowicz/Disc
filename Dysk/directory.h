#pragma once
#include <vector>

class directory
{
public:
	directory();

	void createDir(std::string name, short upDir = -1);
	void addToDir(short InodeNum);
	void remoweInodeNum(short inode);
	void deleteDir();

	std::vector<short> getInodes();
	void setName(std::string name);
	void setUpDir(short num);
	void setLowDir(short num);
	short getLowDir();
	short getUpDir();
	std::string getName();
	void printDir();
private:
	std::vector<short> dataInDirecotry; // Wska�niki na Iw�z�y plik�w zawartych w danym folderze
	std::string name;
	short upDir; // "Wska�nik" na katalog nadrz�dny
	short lowDir; // "Wska�nik" na folder podrz�dny

	
};
