#include "directory.h"
#include <iostream>

directory::directory()
{
	this->name = "";
	this->upDir = -1;
	this->lowDir = -1;
}

void directory::createDir(std::string name, short upDir)
{
	this->name = name;
	this->upDir = upDir;
	this->lowDir = -1; // Nie mo�na utworzy� od razu noego foleru z folderem podrz�dnym w nim
}

void directory::addToDir(short InodeNum)
{
	this->dataInDirecotry.push_back(InodeNum); // Wrzyci do wekrotra wybrany i weze�
}

void directory::remoweInodeNum(short inode)
{
	std::vector<short> d; 
	for (int i = 0; i < this->dataInDirecotry.size(); i++)
	{
		if(dataInDirecotry[i]!=inode)  // Tworzy nowy wektor, przepisuje warto�ci ze starego opr�cz warto�ci przekazanej do funckji
		{
			d.push_back(i);
		}
	}
	this->dataInDirecotry = d;
}

void directory::deleteDir()
{
	//this->
}

std::vector<short> directory::getInodes()
{
	return this->dataInDirecotry; // Zwr�ci vektor i w�z��
}

void directory::setName(std::string name)
{
	this->name = name;
}

void directory::setUpDir(short num)
{
	this->upDir = num;
}

void directory::setLowDir(short num)
{
	this->lowDir = num;
}

short directory::getLowDir()
{
	return this->lowDir;
}

short directory::getUpDir()
{
	return this->upDir;
}

std::string directory::getName()
{
	return this->name;
}


void directory::printDir() 
{
	std::clog << "Nazwa: " << getName().c_str() << "\n"; // Pobranie nazwy
	std::clog << "Folder nadrzedny: " << this->upDir << "\n"; 
	std::clog << "Folder podrzedny: " << this->lowDir << "\n";
	std::clog << "Iwezly: \n";
	for (short element : dataInDirecotry) //Za pomoc� p�tli foreach wypisuje na ekrana numery i w�z��w w folderze
	{
		std::clog << element << " ";
	}
	std::clog << "\n";
}

