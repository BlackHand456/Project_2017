#include "chest.h"

chest::chest() : chest::chest("Small Health Potion", "hp", 1, 10, 20, false) {};
	
chest::chest(string iItemName, string iStat, int iVal, int iRow, int iCol, bool iOpened)
{
	itemName = iItemName;
	stat = iStat;
	val = iVal;
	row = iRow;
	col = iCol;
	opened = iOpened;
}