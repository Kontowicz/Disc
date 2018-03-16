#include "Disc.h"
#include <iostream>
#include <bitset>

Disc::Disc()
{
	for(int i=0; i<1024; i++)
	{
		this->discTable[i] = '0'; // Tablica jest wype³niana 0
	}
	this->freeBlocks = 32; // Ustawiam liczbê wolnyc bloków
	directory diskDir; // Tworzê folder do kótrego przypiszê wszystkie iwêz³y 
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
		this->freeBlockVec[block] = 1; // Ustawiam w wektorze bitowym ten blok jako zajêty
		--freeBlocks; // ZMniejsza liczbê wolnych bloków
		this->InodeTable[block].setSize(1); // Ustawiam rozmiar poliku na 1
		this->InodeTable[block].setBlock1(block); // Przypisujê "wskaŸnik" na blok 1
		this->discTable[block * 32] = name; // Zapisujê nazwê pliku na dysku
	}else
	{
		std::clog << "Brak miejsca na dysku!\n"; 
	}
}

void Disc::appendFile(char name, std::string data)
{
	bool help = true;

	short position = findInode(name); // Szukam numeru i wez³a pliku

	for(int i=0; i<data.length(); i++) // Przechodzê po string data 0 do koñca 
	{
		if((this->InodeTable[position].getSize() / 32)==0) // Jeœli 1 blok jest wolny zapisaujê do niego 
		{
			this->discTable[(this->InodeTable[position].getBlock1() * 32) + (this->InodeTable[position].getSize() % 32) ] = data[i]; // Obliczam pozycjê zapisu na dysku i zapisuje w tym miejscu
			(this->InodeTable[position].setSize(this->InodeTable[position].getSize() + 1)); // Zwiêksza wielkoœæ pliku
			if(this->InodeTable[position].getSize() % 32==0) // Potrzebne do usiniêcia b³êdu 
			{
				continue;
			}
		}
	

		if ((this->InodeTable[position].getSize() / 32) == 1) //Jeœli drugi blok jest wolny zapisujê do niego 
		{
			if(this->InodeTable[position].getBlock2()==-1) // Jeœli "wskaŸnik" na 1 blok jest niezainicjalozowany
			{
				short block = this->getFreeBlock(); // SZukam wolnego bloku na dysku
				this->InodeTable[position].setBlock2(block); // Przypisuje numer znalezionego bloku do "wska¿nika" na bolok 2
				this->freeBlockVec[block] = 1; // Uwstawiam w wektorze bitowym jako zajêty
			}

	
				this->discTable[(this->InodeTable[position].getBlock2() * 32) + (this->InodeTable[position].getSize() % 32)] = data[i];  // Obliczam pozycjê zapisu na dysku i zapisuje w tym miejscu
				(this->InodeTable[position].setSize(this->InodeTable[position].getSize() + 1)); // Zwiêksza rozmiar pliku
	
		}

		if((this->InodeTable[position].getSize() / 32) > 1) // Jeœli bloki 1 i 2 s¹ pe³ne to wykouj:
		{
			//SZUKAM MIEJSCA NA BLOK INDEKSOWY
			if(this->InodeTable[position].getPointerToBlock()==-1) // Jeœli jest niezainicjalizowany to:
			{
				short block = this->getFreeBlock(); // Znajdujê wolny blok 
				this->InodeTable[position].setPointerToBlock(block); // Przypisujê go do "wskaŸnika"
				this->freeBlockVec[block] = 1; // Ustawiam go jako zajêty w wektorze bitowym
				for(int p = this->InodeTable[position].getPointerToBlock()*32; p<this->InodeTable[position].getPointerToBlock()*32+32; p++) //Iterujê przez ca³y blok indeksowy i numery ustawiam na -1, aby wiedzieæ gdzie mogê przechowywaæ numery kolejnych bloków 
				{
					this->discTable[p] = -1; 
				}
			}
			// SZUKAJ bloku DO ZAPISU
		
			int k; // Pomocnicza zmienna
			if (this->InodeTable[position].getSize()%32==0) // Jeœli miesce w przydzielonych dot¹d blokach siê koñczy to:
			{
				k = InodeTable[position].getPointerToBlock() * 32; // Pobierasz wskaŸnik na blok indeksowy, i obliczasz pozycjê 1 bitu tego bloku
				for(k; k<k+32; k++) // Iterujesz po ca³ym bloku
				{
					if(this->discTable[k]==-1) // Kiedy znajdziesz 1 wolny bit (==-1) to w nim zapiszesz nowy numer bloku 
					{
						short block = this->getFreeBlock(); // Zapisujê nowy numer bloku
						this->discTable[k] = block; // Przypisuje wskaŸnik to bloki do konkretnego bitu
						this->freeBlockVec[block] = 1; // Ustawiam ten blok jako zajêty

						if(help) // Aby unikn¹æ zapisu dwa razy tej samej litery
						{
							help = false;
							i++;
						}
						break; 
					}
				}
			}
			int l;

			for( l=this->InodeTable[position].getPointerToBlock()*32; l<this->InodeTable[position].getPointerToBlock()*32+32; l++) // Iterujê po "ca³ym" bloku indeksowym
			{
				if(this->discTable[l] != -1 && this->discTable[l+1]==-1) // Aby blok do którego piszê by³ ostatni musi byæ ostatnim wpisamym, a po ostatnim wpisanym na pewno jest -1
				{
					l = static_cast<int>(this->discTable[l]); // Rzutowanie typów aby wys³uskaæ wartoœæ
					break;
				}
			}
			
			this->discTable[l*32 + (this->InodeTable[position].getSize() % 32)] = data[i]; // Obliczam pozycjê zapisu na dysku i zapisuje w tym miejscu
			
			(this->InodeTable[position].setSize(this->InodeTable[position].getSize() + 1)); // Zwiêksza rozmiar pliku
		}
	}
}

void Disc::deleteFile(char name)
{
	short pos = this->findInode(name); // Szukam i wez³a tego pliku

	this->InodeTable[pos].printINode();
		
	this->freeBlockVec[this->InodeTable[pos].getBlock1()] = 0; // Resterujesz w wektorze bitowym pierwszy blok

	if(this->InodeTable[pos].getBlock2()!=-1) // Resetowanie 2 bloku jeœli zajêty
	{
		this->freeBlockVec[this->InodeTable[pos].getBlock2()] = 0;
	}

	// Zwalnianie bloków zapisanych w bloku indeksowym
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
	this->InodeTable[pos].resetINode(); // Resetujesz i wêze³
}

void Disc::changeFileName(char name, char newName)
{
	short pos = this->findInode(name); // Szukam i wez³a tego pliku
	this->discTable[this->InodeTable[pos].getBlock1() * 32] = newName; // Na podstawie wskaŸnika na 1 blok pliku obliczam pozycjê 1 bitu w tablicy dyskowej i zmieniam ja na now¹ nazwê
}

void Disc::createDir(std::string name, std::string nameUpDir)
{
	directory dir; // Tworze katalog
	dir.setName(name); // Ustawiam jego nazwê 

	dir.setUpDir(this->findDirectory(nameUpDir)); // Ustaw folder nadrzêdny
	
	this->directories.push_back(dir); // Dodajê go do kolecji katalogów 
	int p = -1; // Zmienna pomicnicza
	for(int i=0; i<this->directories.size(); i++) // Iterujê po kolejkcji katalogów aby pobraæ numer pozycji katalogu w którym tworzê katalog
	{
		if(directories[i].getName()== name)
		{
			p = i; // Zapusjê tê pozycjê
			break;
		}
	}
	dir.setLowDir(p);// Ustaw folder podrzêdny

	for (int i = 0; i<this->directories.size(); i++)
	{
		if (directories[i].getName() == nameUpDir) //Poszukujê katalogu nazrzêdnego 
		{
			p = i; 
			break;
		}
	}
	this->directories[p].setLowDir(this->findDirectory(name)); // Ustaiwiam nazwê nowego katalogu (na podstawie wskaŸnika z katalogu nadrzêdnego)
}

void Disc::addToDir(std::string name, char nameFile)
{
}

void Disc::addInodeToDir(std::string nameDir, char nameFile)
{
	short InodeNum = this->findInode(nameFile); // Szukam numeru i wez³a dodawanego do katalogu
	// Usuñ iWêze³ z folderu w którym jest przechowywany i przypisz go do nowego 
	int directoryPosition = this->findDirectory(nameDir); // Szukam pozycji katalogu w którym obecnie jest przechowywany
	this->directories[directoryPosition].addToDir(InodeNum); // Dodaje i wêz³y do katalogu
	int g = this->directories[directoryPosition].getUpDir(); // Pobiram pozycjê katalogu nadrzêdnego w stisunku do nameDir
	this->directories[g].remoweInodeNum(InodeNum); // Usuwam z jego kolekcji i wêze³

}

void Disc::deleteDir(std::string name)
{
	// 1. ZnajdŸ w direcotries folder name
	// 2. Przejdz po folderach i zwolnij ich i wêz³y
	// 3. Zwolnij i wez³y 
	int pos = this->findDirectory(name);
	while (this->directories[pos].getLowDir() != -1) // Przechodzenie do najni¿eszgo folderu
	{
		pos = this->directories[pos].getLowDir();
	}

	//int pos1 = this->directories[pos].getLowDir();
	

	while(this->directories[this->directories[pos].getUpDir()].getName() != name ) //Przechodzenie od najni¿szego folderu w górê i uswanie plików (zwalnianie iwêz³ów)
	{
		std::vector<short> we = directories[pos].getInodes(); // Pobieram i wêz³y pliku

		for(int i=0; i<we.size(); i++) // Przechodzê kolejcjê i wêz³ów
		{
			this->freeBlockVec[this->InodeTable[we[i]].getBlock1()] = 0; // Walniam w wektorzerze bitowym
			this->freeBlockVec[this->InodeTable[we[i]].getBlock2()] = 0;

			if(this->freeBlockVec[this->InodeTable[we[i]].getPointerToBlock()] != -1) // Zwalniam blok indeksowy
			{
				for(int ii = this->InodeTable[we[i]].getPointerToBlock()*32; ii<this->InodeTable[we[i]].getPointerToBlock()*32+32; ii++) // Czyszczê blok indeksowy
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

		std::vector<directory> re; // Przechowywanie kolekcji katalogów, przpepisanie jej z pominiêciem uwuwanego katalogu i przypisanie nowego wektora do starego 
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
	// Przchodzê ca³y dysk i wyprowadzam go na ekran
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
	//Wypisanie i wêz³ów nale¿¹cych do folderu
	int num = this->findDirectory(name); 
	std::vector<short> we = this->directories[num].getInodes(); // Metoda getInode zwraca wektor i aby na nim operowaæ trzeba go gdzieœ zapisaæ
	// W we s¹ numery i wez³ow nale¿¹cych do pliku, wypiszemy je
	for(int i=0; i<we.size(); i++)
	{
		this->InodeTable[we[i]].printINode(); // Wypisze szczegó³owe inforamacje 
	}
}

void Disc::printAllFiles()
{
	for(int i =0; i<32; i++) //Przejdzie po ca³ej tablicy i wez³ów
	{
		std::clog << "\n";
		if(this->InodeTable[i].getBlock1()!=-1) // Jeœli i we¿e³ jest zainicjalizowany
		{
			std::clog << "Nazwa: " << this->discTable[this->InodeTable[i].getBlock1() * 32] << "\n"; 
			this->InodeTable[i].printINode(); // Szczegó³owe dane 
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
		if (this->InodeTable[i].getBlock1() != -1) // Iterujê po koleksji i wez³ów, jak znajdê w³aœciwy i wêze³ to koñczê pracê i wracam ten numer 
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
{ // Zwraca pierwszy wolny blok który napotka 
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
	for (int i = 0; i < this->directories.size(); i++) // Przechodzê po kolejkcji folderów 
	{
		if(this->directories[i].getName()==name)  // Jeœli imiê któregoœ jest zgodne z przekazanym do metody wtedy wracam jego numer w kolekcji 
		{
			p = i;
			break;
		}
	}
	return p;
}
