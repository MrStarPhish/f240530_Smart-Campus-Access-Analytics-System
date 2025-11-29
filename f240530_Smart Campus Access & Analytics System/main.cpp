// 24F-0530 - Abdullah Saleem - Smart Campus System 
//  main here



#include<iostream>
#include<conio.h>
#include"System.h"


using namespace std;

int main()
{
	SetConsoleWindowSize(WIDTH, HEIGHT);
	//SetConsoleBufferSize(125, 40);
	
	System sys;


	if (sys.initialize())
	{
		sys.start();  // <--- Ignition
	}
	else 
	{
		sys.generateErrorResponse("System failed to initialize. Exiting...");
	}

	

	cout << endl << endl;
	sys.insertCommandManually("/restart");
	sys.generateSystemResponse("Program Terminated. [Press any key to exit]...");

	_getch();
	return 0;
}