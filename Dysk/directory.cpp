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
	this->lowDir = -1; // Nie mo¿na utworzyæ od razu noego foleru z folderem podrzêdnym w nim
}

void directory::addToDir(short InodeNum)
{
	this->dataInDirecotry.push_back(InodeNum); // Wrzyci do wekrotra wybrany i weze³
}

void directory::remoweInodeNum(short inode)
{
	std::vector<short> d; 
	for (int i = 0; i < this->dataInDirecotry.size(); i++)
	{
		if(dataInDirecotry[i]!=inode)  // Tworzy nowy wektor, przepisuje wartoœci ze starego oprócz wartoœci przekazanej do funckji
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
	return this->dataInDirecotry; // Zwróci vektor i wêz³ó
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
	for (short element : dataInDirecotry) //Za pomoc¹ pêtli foreach wypisuje na ekrana numery i wêz³ów w folderze
	{
		std::clog << element << " ";
	}
	std::clog << "\n";
}

