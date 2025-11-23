// 24F-0530  main here



#include<iostream>
#include<conio.h>
#include"CLASSES.h"


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
	cout << "Program Terminated. [Press any key to exit]...";

	_getch();
	return 0;
}