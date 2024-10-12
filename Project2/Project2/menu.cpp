#include <iostream>
using namespace std;

/***************
*Author: Dominic Nguyen
*Creation Date: 11/14/2023
*Modification Date: 12/5/2023
*Purpose:  Displays a menu with 7 options that returns their choice
*		   -checks inputs as well.
****************/

int menu()
{
	cout << "-------------------------Menu---------------------------\n"
		<< "1. Display Seat Chart\n"
		<< "2. Reserve Seat\n"
		<< "3. Cancel Reservation\n"
		<< "4. Save Seat Chart to File\n"
		<< "5. Statistics\n"
		<< "6. Help\n"
		<< "7. Quit\n"
		<< "--------------------------------------------------------\n"
		<< "Please Enter Your Choice (1-7): ";

	int choice;
	cin >> choice;
	while (cin.fail() || choice > 7 || choice < 1)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Please Enter Your Choice (1-7): ";
		cin >> choice;
	}
	cout << endl;

	return choice;
}