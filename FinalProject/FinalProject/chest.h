#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

class chest
{
public:
	string itemName;
	string stat;
	int val;
	int row;
	int col;
	bool opened;

	chest();
	chest(string iItemName, string iStat, int iVal, int iRow, int iCol, bool iOpened);
};
