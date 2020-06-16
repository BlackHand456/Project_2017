#pragma once
#include "character.h"
#include <windows.h>

class player : public character
{
public:
	player(string iName, int iStr, int iDef, int iMaxHp, int iHp, int iMaxMp, int iMp, int iRow, int iCol, bool iKey) : character(iName,iStr,iDef,iMaxHp,iHp,iMaxMp,iMp,iRow,iCol,iKey) {};
	virtual void attack(character *c);
	virtual void move();
	virtual bool isAlive();
	virtual char getLast();
	bool getQuit();
	bool getInStats();
	void setInStats(bool a);
};
