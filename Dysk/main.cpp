#include  <iostream>
#include "Disc.h"
#include <conio.h>



int main()
{
	Disc d;
	//bool isOk = true;
	
	/*int wybor = 0;
	do
	{
		std::cout << "Wybierz opcje:\n0.Zakoncz\n1.Utworz plik\n2.Zapisz do pliku3.\n3.Usun plik.\n4.Wypisz dysk\n";
		std::cin >> wybor;

		switch (wybor)
		{
		case 0:
			{
			exit(EXIT_SUCCESS);
			}
			break;
		case  1:
			{
			char name; 
			std::cout << "podaj nazwe pliku (1 litera)\n";
			
			std::cin >> name;
		
			d.createFile(name);
			_getch();
			system("cls");
			}
			break;
		case 2:
			{
			char name;
			std::string dane;
			std::clog << "Podaj dane do zapisania\n";
			std::cin >> dane;
			std::clog << "\nPodaj nazwe pliku d ktorego chcesz je dopisac\n";
			std::cin >> name;
			d.appendFile(name, dane);


			_getch();
			system("cls");
			}
			break;
		case 3:
			{
			char name;
			std::clog << "Podaj nazwe pliku do usuniecia\n";
			std::cin >> name;
			d.deleteFile(name);


			_getch();
			system("cls");
			}
			break;
		case 4:
			{
			d.printDisc();


			_getch();
			system("cls");
			}
			break;
		
		
		}
	} while (wybor != 0);

	std::clog << "\n\n";*/

	d.createFile('a');
	d.appendFile('a', "0123456789012345671234567890123456789012345678901234567890123456712345678901234567890123456789");
	d.printFreeBlock();
	std::clog << "\n\n";
	d.printDisc();
	d.deleteFile('a');
	std::clog << "\n\n";
	d.printFreeBlock();
	std::clog << "\n\n";
	d.printAllFiles();
	std::clog << "\n\n";
	d.printDisc();
	return 0;
}
