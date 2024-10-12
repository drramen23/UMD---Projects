#include "classes.h"
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

Theater::Theater()
{
	readChart();
}

void Theater::readChart()
{
	ifstream inFile;
	inFile.open("chartIn.txt");
	if (!inFile.is_open())
	{
		cout << "ERROR: Failure to open \"chartIn.txt\"" << endl;
		return;
	}


	int rowNum;
	char seatChar;

	for (int r = 0; r < ROW; ++r)
	{
		inFile >> rowNum;
		for (int c = 1; c < COL; ++c)
		{
			inFile >> seatChar;
			seatChart[r][c] = Seat(rowNum, seatChar);
		}
	}

	inFile.close();
}

/****************************************************************
							 MENU METHODS
****************************************************************/

void Theater::showChart()
{
	//shows chart saved on the theater array of seat
	for (int r = 0; r < ROW; ++r)
	{
		cout << left << setw(2) << setfill(' ') << (r + 1) << " ";
		for (int c = 1; c < COL; ++c)
		{
			cout << (seatChart[r][c].stateReserved() ? 'X' : seatChart[r][c].getSeatNum());
			cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Theater::reserveSeat()
{
	int rowNum;
	char seatNum;
	string seat;

	cout << "Enter the seat that you wish to reserve: ";
	cin >> seat;

	while (cin.fail() || seat.size() < 2)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid input.\n"
			<< "Enter the seat that you wish to reserve : ";
		cin >> seat;
	}

		std::istringstream(seat) >> rowNum;
		seatNum = toupper(seat.at(seat.size()-1));

	if (rowNum >= 1 && rowNum <= ROW && seatNum >= 'A' && seatNum <= ('A' + (COL - 2)))
	{
		int rowIdx = rowNum - 1;
		int seatIdx = seatNum - 64;

		if (!seatChart[rowIdx][seatIdx].stateReserved())
		{
			seatChart[rowIdx][seatIdx].reserve();
			cout << "Seat " << seat << " reserved.\n\n";
		}
		else
			cout << "Seat" << seat << " is already reserved\n\n";
	}
	else
		cout << "Invalid input\n\n";
}

void Theater::cancelSeat()
{
	int rowNum;
	char seatNum;
	string seat;

	cout << "Enter the seat that you wish to cancel: ";
	cin >> seat;

	while (cin.fail() || seat.size() < 2)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		cout << "Invalid input.\n"
			<< "Enter the seat that you wish to cancel : ";
		cin >> seat;
	}

	std::istringstream(seat) >> rowNum;
	seatNum = toupper(seat.at(seat.size() - 1));

	if (rowNum >= 1 && rowNum <= ROW && seatNum >= 'A' && seatNum <= ('A' + (COL - 2)))
	{
		int rowIdx = rowNum - 1;
		int seatIdx = seatNum - 64;

		if (seatChart[rowIdx][seatIdx].stateReserved())
		{
			seatChart[rowIdx][seatIdx].cancel();
			cout << "Seat " << seat << " canceled.\n\n";
		}
		else
			cout << "Seat " << seat << " has not been reserved yet.\n\n";
	}
	else
		cout << "Invalid input\n\n";
}

void Theater::saveToFile()
{
	ofstream outFile;
	string userFile;

	cout << "Please enter a file name to save the seating chart to.\n";
	cin >> userFile;

	outFile.open(userFile);
	if (!outFile.is_open())
	{
		cout << "ERROR: Failure to create \"" << userFile << endl;
		return;
	}

	//writes array to file in format
	for (int r = 0; r < ROW; ++r)
	{
		outFile << left << setw(2) << setfill(' ') << (r + 1) << " ";
		for (int c = 1; c < COL; ++c)
		{
			outFile << (seatChart[r][c].stateReserved() ? 'X' : seatChart[r][c].getSeatNum());
			outFile << " ";
		}
		outFile << endl;
	}
	outFile << endl;

	outFile.close();
}

void Theater::statistics()
{
	int numSeats = 0;
	int seatsAvailable = 0;
	int seatsReserved = 0;
	float percentReserved = 0;

	//counts the number of reserved seats and keeps track of seats counted
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 1; c < COL; ++c)
		{
			++numSeats;
			if (seatChart[r][c].stateReserved())
				++seatsReserved;
		}
	}

	percentReserved =  100 * static_cast<float>(seatsReserved) / numSeats;
	seatsAvailable = numSeats - seatsReserved;

	cout << seatsAvailable << " seats available." << endl;
	cout << percentReserved << " percent of seats are reserved." << endl << endl;


	//LIST OF AISLE SEATS
	cout << "Aisle seats available:\n";

	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 1; c < COL; c += (COL-2))
		{
			if (!seatChart[r][c].stateReserved())
			{
				cout << seatChart[r][c].getRowNum();
				cout << seatChart[r][c].getSeatNum();
				cout << " ";
			}
		}
		cout << endl;
	}
	cout << endl;


	//LIST OF INTERIOR SEATS
	cout << "Interior seats available:\n";
	for (int r = 0; r < ROW; ++r)
	{
		for (int c = 2; c < (COL-1); ++c)
		{
			if (!seatChart[r][c].stateReserved())
			{
				cout << seatChart[r][c].getRowNum();
				cout << seatChart[r][c].getSeatNum();
				cout << " ";
			}
		}
		cout << endl;
	}
	cout << endl;

}
