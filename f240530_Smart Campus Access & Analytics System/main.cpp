// 24F-0530  main here



#include<iostream>
#include<conio.h>
#include"CLASSES.h"


using namespace std;

int main()
{
	SetConsoleWindowSize(125, 40);
	//SetConsoleBufferSize(125, 40);
	drawBoundary();
	gotoxy(0,0);

	System sys;

	sys.loadUsers();
	sys.loadDoors();


	

	cout << endl << endl;
	cout << "Program Terminated. [Press any key to exit]...";

	_getch();
	return 0;
}