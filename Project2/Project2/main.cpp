#include <iostream>
#include <fstream>
#include "classes.h"
#include "functions.h"
using namespace std;

/***************
*Author: Dominic Nguyen
*Creation Date: 11/14/2023
*Modification Date: 12/5/2023 
*Purpose:  Models a theater with 12x15 seats.
*		   -able to reserve and cancel seats
****************/

int main()
{
	Theater theater;
	bool quit = false;


	while (!quit) //repeats until user wants to quit
	{
		switch (menu())
		{
		case 1:
			theater.showChart();
			break;
		case 2:
			theater.reserveSeat();
			break;
		case 3:
			theater.cancelSeat();
			break;
		case 4:
			theater.saveToFile();
			break;
		case 5:
			theater.statistics();
			break;
		case 6:
			help();
			break;
		case 7:
			cout << "Thanks for using TicketMister";
			quit = true;
			break;
		default:
			cout << "Menu Return Error";
		}
	}

	cout << endl << endl;
	system("pause");
	return 0;
}