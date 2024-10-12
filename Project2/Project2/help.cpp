#include <iostream>

/***************
*Author: Dominic Nguyen
*Creation Date: 11/14/2023
*Modification Date: 12/5/2023
*Purpose:  Displaya a help message to guide the user on all of the options.
****************/

void help()
{
	std::cout
		<< "------------------------Help---------------------------\n"
		<< "1. Displays the seating chart of the theater.\n\n"

		<< "2. The program will ask you to type in a seat \n"
		<< "   (e.g. 3H) that you wish to register. If the\n"
		<< "   seat is available, it will place an 'X' on\n"
		<< "   the seat and confirm that it was reserved.\n"
		<< "   Otherwise, a message will show stating that\n"
		<< "   seat isn't available.\n\n"

		<< "3. The program will ask you to type in a seat\n"
		<< "   (e.g. 1A) that you wish to cancel registration\n"
		<< "   for. If the seat is registered, it will become\n"
		<< "   available and output a confirmation. Otherwise,\n"
		<< "   if the seat was already available, an error\n"
		<< "   message will appear.\n\n"

		<< "4. The program will save the seat chart to a file\n"
		<< "   that is typed in by the user is the format\n"
		<< "   \"FILENAME.txt\" \n\n"

		<< "5. The program will display the following stats:\n"
		<< "   -number of available seats\n"
		<< "   -percentage of seats that are reserved\n"
		<< "   -list of aisle seats that are available\n"
		<< "   -list of interior seats that are available\n\n"

		<< "6. Shows this detailed message on how to use each\n"
		<< "   of the menu options in the program.\n\n"

		<< "7. The program displays a thank you message and ends.\n\n";

	return;
}