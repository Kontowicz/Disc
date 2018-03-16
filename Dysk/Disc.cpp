#include "Disc.h"
#include <iostream>
#include <bitset>

Disc::Disc()
{
	for(int i=0; i<1024; i++)
	{
		this->discTable[i] = '0'; // Tablica jest wype�niana 0
	}
	this->freeBlocks = 32; // Ustawiam liczb� wolnyc blok�w
	directory diskDir; // Tworz� folder do k�trego przypisz� wszystkie iw�z�y 
	diskDir.setName("Dysk");
	for (short i = 0; i<32; i++)
	{
		diskDir.addToDir(i);
	}
	this->directories.push_back(diskDir);
}

void Disc::createFile(char name)
{
	if(freeBlocks)
	{
		short block = this->getFreeBlock(); // Znajduje wolny blok 
		this->freeBlockVec[block] = 1; // Ustawiam w wektorze bitowym ten blok jako zaj�ty
		--freeBlocks; // ZMniejsza liczb� wolnych blok�w
		this->InodeTable[block].setSize(1); // Ustawiam rozmiar poliku na 1
		this->InodeTable[block].setBlock1(block); // Przypisuj� "wska�nik" na blok 1
		this->discTable[block * 32] = name; // Zapisuj� nazw� pliku na dysku
	}else
	{
		std::clog << "Brak miejsca na dysku!\n"; 
	}
}

void Disc::appendFile(char name, std::string data)
{
	bool help = true;

	short position = findInode(name); // Szukam numeru i wez�a pliku

	for(int i=0; i<data.length(); i++) // Przechodz� po string data 0 do ko�ca 
	{
		if((this->InodeTable[position].getSize() / 32)==0) // Je�li 1 blok jest wolny zapisauj� do niego 
		{
			this->discTable[(this->InodeTable[position].getBlock1() * 32) + (this->InodeTable[position].getSize() % 32) ] = data[i]; // Obliczam pozycj� zapisu na dysku i zapisuje w tym miejscu
			(this->InodeTable[position].setSize(this->InodeTable[position].getSize() + 1)); // Zwi�ksza wielko�� pliku
			if(this->InodeTable[position].getSize() % 32==0) // Potrzebne do usini�cia b��du 
			{
				continue;
			}
		}
	

		if ((this->InodeTable[position].getSize() / 32) == 1) //Je�li drugi blok jest wolny zapisuj� do niego 
		{
			if(this->InodeTable[position].getBlock2()==-1) // Je�li "wska�nik" na 1 blok jest niezainicjalozowany
			{
				short block = this->getFreeBlock(); // SZukam wolnego bloku na dysku
				this->InodeTable[position].setBlock2(block); // Przypisuje numer znalezionego bloku do "wska�nika" na bolok 2
				this->freeBlockVec[block] = 1; // Uwstawiam w wektorze bitowym jako zaj�ty
			}

	
				this->discTable[(this->InodeTable[position].getBlock2() * 32) + (this->InodeTable[position].getSize() % 32)] = data[i];  // Obliczam pozycj� zapisu na dysku i zapisuje w tym miejscu
				(this->InodeTable[position].setSize(this->InodeTable[position].getSize() + 1)); // Zwi�ksza rozmiar pliku
	
		}

		if((this->InodeTable[position].getSize() / 32) > 1) // Je�li bloki 1 i 2 s� pe�ne to wykouj:
		{
			//SZUKAM MIEJSCA NA BLOK INDEKSOWY
			if(this->InodeTable[position].getPointerToBlock()==-1) // Je�li jest niezainicjalizowany to:
			{
				short block = this->getFreeBlock(); // Znajduj� wolny blok 
				this->InodeTable[position].setPointerToBlock(block); // Przypisuj� go do "wska�nika"
				this->freeBlockVec[block] = 1; // Ustawiam go jako zaj�ty w wektorze bitowym
				for(int p = this->InodeTable[position].getPointerToBlock()*32; p<this->InodeTable[position].getPointerToBlock()*32+32; p++) //Iteruj� przez ca�y blok indeksowy i numery ustawiam na -1, aby wiedzie� gdzie mog� przechowywa� numery kolejnych blok�w 
				{
					this->discTable[p] = -1; 
				}
			}
			// SZUKAJ bloku DO ZAPISU
		
			int k; // Pomocnicza zmienna
			if (this->InodeTable[position].getSize()%32==0) // Je�li miesce w przydzielonych dot�d blokach si� ko�czy to:
			{
				k = InodeTable[position].getPointerToBlock() * 32; // Pobierasz wska�nik na blok indeksowy, i obliczasz pozycj� 1 bitu tego bloku
				for(k; k<k+32; k++) // Iterujesz po ca�ym bloku
				{
					if(this->discTable[k]==-1) // Kiedy znajdziesz 1 wolny bit (==-1) to w nim zapiszesz nowy numer bloku 
					{
						short block = this->getFreeBlock(); // Zapisuj� nowy numer bloku
						this->discTable[k] = block; // Przypisuje wska�nik to bloki do konkretnego bitu
						this->freeBlockVec[block] = 1; // Ustawiam ten blok jako zaj�ty

						if(help) // Aby unikn�� zapisu dwa razy tej samej litery
						{
							help = false;
							i++;
						}
						break; 
					}
				}
			}
			int l;

			for( l=this->InodeTable[position].getPointerToBlock()*32; l<this->InodeTable[position].getPointerToBlock()*32+32; l++) // Iteruj� po "ca�ym" bloku indeksowym
			{
				if(this->discTable[l] != -1 && this->discTable[l+1]==-1) // Aby blok do kt�rego pisz� by� ostatni musi by� ostatnim wpisamym, a po ostatnim wpisanym na pewno jest -1
				{
					l = static_cast<int>(this->discTable[l]); // Rzutowanie typ�w aby wys�uska� warto��
					break;
				}
			}
			
			this->discTable[l*32 + (this->InodeTable[position].getSize() % 32)] = data[i]; // Obliczam pozycj� zapisu na dysku i zapisuje w tym miejscu
			
			(this->InodeTable[position].setSize(this->InodeTable[position].getSize() + 1)); // Zwi�ksza rozmiar pliku
		}
	}
}

void Disc::deleteFile(char name)
{
	short pos = this->findInode(name); // Szukam i wez�a tego pliku

	this->InodeTable[pos].printINode();
		
	this->freeBlockVec[this->InodeTable[pos].getBlock1()] = 0; // Resterujesz w wektorze bitowym pierwszy blok

	if(this->InodeTable[pos].getBlock2()!=-1) // Resetowanie 2 bloku je�li zaj�ty
	{
		this->freeBlockVec[this->InodeTable[pos].getBlock2()] = 0;
	}

	// Zwalnianie blok�w zapisanych w bloku indeksowym
	if (this->InodeTable[pos].getPointerToBlock() != -1) {
		int toDelete;
		toDelete = this->InodeTable[pos].getPointerToBlock() * 32;
		this->freeBlockVec[toDelete / 32] = 0;

		if (this->freeBlockVec[this->InodeTable[pos].getPointerToBlock()] != -1)
		{

			for (int i = toDelete; i < toDelete + 32; i++)
			{
				if (this->discTable[i] != -1)
				{
					this->freeBlockVec[this->discTable[i]] = 0;
				}
				this->discTable[i] = '0';
			}
		}
	}
	this->InodeTable[pos].resetINode(); // Resetujesz i w�ze�
}

void Disc::changeFileName(char name, char newName)
{
	short pos = this->findInode(name); // Szukam i wez�a tego pliku
	this->discTable[this->InodeTable[pos].getBlock1() * 32] = newName; // Na podstawie wska�nika na 1 blok pliku obliczam pozycj� 1 bitu w tablicy dyskowej i zmieniam ja na now� nazw�
}

void Disc::createDir(std::string name, std::string nameUpDir)
{
	directory dir; // Tworze katalog
	dir.setName(name); // Ustawiam jego nazw� 

	dir.setUpDir(this->findDirectory(nameUpDir)); // Ustaw folder nadrz�dny
	
	this->directories.push_back(dir); // Dodaj� go do kolecji katalog�w 
	int p = -1; // Zmienna pomicnicza
	for(int i=0; i<this->directories.size(); i++) // Iteruj� po kolejkcji katalog�w aby pobra� numer pozycji katalogu w kt�rym tworz� katalog
	{
		if(directories[i].getName()== name)
		{
			p = i; // Zapusj� t� pozycj�
			break;
		}
	}
	dir.setLowDir(p);// Ustaw folder podrz�dny

	for (int i = 0; i<this->directories.size(); i++)
	{
		if (directories[i].getName() == nameUpDir) //Poszukuj� katalogu nazrz�dnego 
		{
			p = i; 
			break;
		}
	}
	this->directories[p].setLowDir(this->findDirectory(name)); // Ustaiwiam nazw� nowego katalogu (na podstawie wska�nika z katalogu nadrz�dnego)
}

void Disc::addToDir(std::string name, char nameFile)
{
}

void Disc::addInodeToDir(std::string nameDir, char nameFile)
{
	short InodeNum = this->findInode(nameFile); // Szukam numeru i wez�a dodawanego do katalogu
	// Usu� iW�ze� z folderu w kt�rym jest przechowywany i przypisz go do nowego 
	int directoryPosition = this->findDirectory(nameDir); // Szukam pozycji katalogu w kt�rym obecnie jest przechowywany
	this->directories[directoryPosition].addToDir(InodeNum); // Dodaje i w�z�y do katalogu
	int g = this->directories[directoryPosition].getUpDir(); // Pobiram pozycj� katalogu nadrz�dnego w stisunku do nameDir
	this->directories[g].remoweInodeNum(InodeNum); // Usuwam z jego kolekcji i w�ze�

}

void Disc::deleteDir(std::string name)
{
	// 1. Znajd� w direcotries folder name
	// 2. Przejdz po folderach i zwolnij ich i w�z�y
	// 3. Zwolnij i wez�y 
	int pos = this->findDirectory(name);
	while (this->directories[pos].getLowDir() != -1) // Przechodzenie do najni�eszgo folderu
	{
		pos = this->directories[pos].getLowDir();
	}

	//int pos1 = this->directories[pos].getLowDir();
	

	while(this->directories[this->directories[pos].getUpDir()].getName() != name ) //Przechodzenie od najni�szego folderu w g�r� i uswanie plik�w (zwalnianie iw�z��w)
	{
		std::vector<short> we = directories[pos].getInodes(); // Pobieram i w�z�y pliku

		for(int i=0; i<we.size(); i++) // Przechodz� kolejcj� i w�z��w
		{
			this->freeBlockVec[this->InodeTable[we[i]].getBlock1()] = 0; // Walniam w wektorzerze bitowym
			this->freeBlockVec[this->InodeTable[we[i]].getBlock2()] = 0;

			if(this->freeBlockVec[this->InodeTable[we[i]].getPointerToBlock()] != -1) // Zwalniam blok indeksowy
			{
				for(int ii = this->InodeTable[we[i]].getPointerToBlock()*32; ii<this->InodeTable[we[i]].getPointerToBlock()*32+32; ii++) // Czyszcz� blok indeksowy
				{
					if(this->discTable[ii]!=-1)
					{
						this->freeBlockVec[this->discTable[ii]] = 0;
					}
				}
			}
		}
		int moment = this->directories[pos].getUpDir(); // Zmienna pomocnicza

		
			pos = this->directories[pos].getUpDir(); // ZMienna pomocnicza 

		std::vector<directory> re; // Przechowywanie kolekcji katalog�w, przpepisanie jej z pomini�ciem uwuwanego katalogu i przypisanie nowego wektora do starego 
		for(int q = 0; q<this->directories.size(); q++)
		{
			if(q!=moment)
			{
				re.push_back(this->directories[q]);
			}
		}
		this->directories = re;
		//this->directories.erase(std::find(this->directories.begin(), this->directories.end(), this->directories[pos].getLowDir()));
	}

	if (this->directories[pos].getName()!="Dysk") {
		std::vector<short> we = directories[pos].getInodes();

		for (int i = 0; i < we.size(); i++)
		{
			this->freeBlockVec[this->InodeTable[we[i]].getBlock1()] = 0;
			this->freeBlockVec[this->InodeTable[we[i]].getBlock2()] = 0;

			if (this->freeBlockVec[this->InodeTable[we[i]].getPointerToBlock()] != -1)
			{
				for (int ii = this->InodeTable[we[i]].getPointerToBlock() * 32; ii < this->InodeTable[we[i]].getPointerToBlock() * 32 + 32; ii++)
				{
					if (this->discTable[ii] != -1)
					{
						this->freeBlockVec[this->discTable[ii]] = 0;
					}
				}
			}
		}
		int moment = this->directories[pos].getUpDir();
		//pos = this->directories[pos].getUpDir();
		std::vector<directory> re;
		for (int q = 0; q < this->directories.size(); q++)
		{
			if (q != moment)
			{
				re.push_back(this->directories[q]);
			}
		}
		this->directories = re;
	}
}

void Disc::printDisc()
{
	// Przchodz� ca�y dysk i wyprowadzam go na ekran
	for(int i=0; i<32; i++) // Ta "pokazuje" bloki 
	{
		std::clog << i <<" ";
		for(int k = 0; k<32; k++)  // Wypisanie danych z bloku
		{
			std::clog << this->discTable[(32 * i) + k];
		}
		std::clog << "\n";
	}
}

void Disc::printDir()
{
	// Wypisanie informacji o wszystkich folderach przechowywanych
	for(int i =0; i<directories.size(); i++)
	{
		directories[i].printDir();
	}
}

void Disc::printFreeBlock()
{
	// Wypisanie waktora bitowego
	std::clog << "wolne bloki: \n";
	for (int i = 0; i<this->freeBlockVec.size(); i++) // 
	{
		std::clog << this->freeBlockVec[i] << " ";
	}
	std::clog << "\n";
}

void Disc::printInodeInDir(std::string name)
{
	//Wypisanie i w�z��w nale��cych do folderu
	int num = this->findDirectory(name); 
	std::vector<short> we = this->directories[num].getInodes(); // Metoda getInode zwraca wektor i aby na nim operowa� trzeba go gdzie� zapisa�
	// W we s� numery i wez�ow nale��cych do pliku, wypiszemy je
	for(int i=0; i<we.size(); i++)
	{
		this->InodeTable[we[i]].printINode(); // Wypisze szczeg�owe inforamacje 
	}
}

void Disc::printAllFiles()
{
	for(int i =0; i<32; i++) //Przejdzie po ca�ej tablicy i wez��w
	{
		std::clog << "\n";
		if(this->InodeTable[i].getBlock1()!=-1) // Je�li i we�e� jest zainicjalizowany
		{
			std::clog << "Nazwa: " << this->discTable[this->InodeTable[i].getBlock1() * 32] << "\n"; 
			this->InodeTable[i].printINode(); // Szczeg�owe dane 
		}
	}
}

void Disc::setDirName(std::string name, std::string newName)
{
	int pos = this->findDirectory(name);
	this->directories[pos].setName(newName);
}

short Disc::findInode(char name)
{
	short l = -1;
	for (short i = 0; i<32; i++)
	{
		if (this->InodeTable[i].getBlock1() != -1) // Iteruj� po koleksji i wez��w, jak znajd� w�a�ciwy i w�ze� to ko�cz� prac� i wracam ten numer 
		{
			if (discTable[i * 32] == name)
			{
				l = i;
				break;
			}
		}
	}
	return l;
}

short Disc::getFreeBlock()
{ // Zwraca pierwszy wolny blok kt�ry napotka 
	short i = 0;
	for(i; i<32; i++)
	{
		if(freeBlockVec[i]==0)
		{
			break;
		}
	}
	return i;
}

short Disc::findDirectory(std::string name)
{
	int p = -1;
	for (int i = 0; i < this->directories.size(); i++) // Przechodz� po kolejkcji folder�w 
	{
		if(this->directories[i].getName()==name)  // Je�li imi� kt�rego� jest zgodne z przekazanym do metody wtedy wracam jego numer w kolekcji 
		{
			p = i;
			break;
		}
	}
	return p;
}
