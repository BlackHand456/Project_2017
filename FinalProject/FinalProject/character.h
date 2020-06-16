#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

class character
{
public:
	string name;
	int str;
	int def;
	int maxhp;
	int hp;
	int maxmp;
	int mp;
	int row;
	int col;
	bool key;

	character();
	character(string iName, int iStr, int iDef, int iMaxHp, int iHp, int iMaxMp, int iMp, int iRow, int iCol, bool iKey);

	virtual void attack(character *c) = 0;
	virtual void move() = 0;
	virtual bool isAlive() = 0;
	virtual char getLast() = 0;
};
