#pragma once
#include <string>
#include <array>
class inode
{
public:
	inode();
	int getSize();
	void setSize(short size);

	short getBlock1();
	short getBlock2();
	void setBlock1(short first = -1); 
	void setBlock2(short second = -1);

	short getPointerToBlock(); 
	void setPointerToBlock(short block);
	void printINode();

	void resetINode();

private:
	int size;	//rozmiar zapisany w liczbie bit�w 
	short block1;
	short block2;// Liczba blokow zajmowana przez plik
	short pointerToBlock; //  Wska�nik na blok indeksowy (jest to jeden z blok�w dyskowych, ale przeznaczony wy��cznie na bycie blokiem indeskowym,
};
