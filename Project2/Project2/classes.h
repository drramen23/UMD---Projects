#pragma once
#include "functions.h"
#include <string>
#include <iostream>
using namespace std;

const int ROW = 12;
const int COL = 16;

class Seat {
private:
	bool reserved;
	int row;
	char num;
public:
	Seat(int Row = -1, char column = '*');

	int getRowNum() { return row; };
	char getSeatNum() { return num; };

	void reserve() { reserved = true; };
	void cancel() { reserved = false; };
	bool stateReserved() { return reserved; };

};

class Theater {
private:
	Seat seatChart[ROW][COL];
public:
	Theater();
	void readChart();
	void showChart();

	void reserveSeat();
	void cancelSeat();
	void saveToFile();
	void statistics();
};

