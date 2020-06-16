#include "character.h"

character::character() : character::character("Default Name", 5, 5, 10, 10, 0, 0, 17, 1, false) {};

character::character(string enteredName, int iStr, int iDef, int iMaxHp, int iHp, int iMaxMp, int iMp, int iRow, int iCol, bool iKey)
{
	name = enteredName;
	str = iStr;
	def = iDef;
	maxhp =	iMaxHp;
	hp = iHp;
	maxmp = iMaxMp;
	mp = iMp;
	row = iRow;
	col = iCol;
	key = iKey;
}