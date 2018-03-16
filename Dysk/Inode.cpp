#include "Inode.h"
#include <iostream>

inode::inode()
{
	this->size = 0;
	this->block1 = -1;
	this->block2 = -1;
	this->pointerToBlock = -1;
}

#pragma region doWyrzucenia
int inode::getSize()
{
	return this->size;
}

void inode::setSize(short size)
{
	this->size = size;
}

short inode::getBlock1()
{
	return this->block1;
}

short inode::getBlock2()
{
	return this->block2;
}



void inode::setBlock1(short first)
{
	this->block1 = first;
}

void inode::setBlock2(short second)
{
	this->block2 = second;
}

short inode::getPointerToBlock()
{
	return this->pointerToBlock;
}

void inode::setPointerToBlock(short block)
{
	this->pointerToBlock = block;
}
#pragma  endregion doWyrzucenia

void inode::printINode()
{
	std::clog << "Pierwszy wskaznik na blok: " << this->block1 << "\n";
	std::clog << "Drugi wskaznik na blok: " << this->block2 << "\n";
	std::clog << "Blok indeksowy: " << this->pointerToBlock << "\n";
	std::clog << "Rozmiar w bajtach: " << this->size << "\n";
}

void inode::resetINode()
{
	this->block1 = -1;
	this->block2 = -1;
	this->pointerToBlock = -1;
	this->size = 0;
}
