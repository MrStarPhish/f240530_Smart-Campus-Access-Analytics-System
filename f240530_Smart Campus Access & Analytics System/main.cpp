// 24F-0530  main here



#include<iostream>
#include<conio.h>
#include"CLASSES.h"


using namespace std;

int main()
{


	System sys;

	sys.loadUsers();
	sys.loadDoors();

	cout << "Smart Campus System Initialized.\n";






	cout << endl << endl;
	cout << "Program Terminated. [Press any key to exit]...";
	_getch();
	return 0;
}