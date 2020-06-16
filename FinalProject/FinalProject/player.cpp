#include "player.h"

char plast;
bool quit = false;

bool inStats = false;
bool pcrit = false;

int KeyEvents = 0;

void player::attack(character *c)
{
	pcrit = false;
	int input;
	cout << "What would you like to do." << endl;
	cout << "<1> Melee\n"
		"<2> Magic" << endl;
	cin >> input;
	if (!cin.fail())
	{
		random_device rd;
		uniform_int_distribution<int> d4(1, 4);
		uniform_int_distribution<int> d10(1, 10);
		uniform_int_distribution<int> d5(1, 5);


		if (input == 1)
		{
			if (d4(rd) != 4)
			{
				int atk;
				if (d10(rd) != 1)
					atk = str - c->def;
				else
				{
					atk = str - c->def;
					if (atk <= 1)
						atk += 1;
					else
						atk += atk / 2;
					pcrit = true;
				}
				if (atk <= 0)
					atk = 1;
				c->hp = c->hp - atk;
				if (pcrit)
				{
					cout << name << " hit the " << c->name << " with a critical hit for " << atk << " damage." << endl;
					cout << "The " << c->name << " now has " << c->hp << "/" << c->maxhp << " hp." << endl;
				}
				else
				{
					cout << name << " hit the " << c->name << " for " << atk << " damage." << endl;
					cout << "The " << c->name << " now has " << c->hp << "/" << c->maxhp << " hp." << endl;
				}
			}
			else
				cout << name << "'s attack missed." << endl;
		}
		else if (input == 2)//make magic
		{
			if (mp > 0)
			{
				if (d5(rd) == 1)
				{
					int atk;
					if (d10(rd) == 1)
					{
						atk = str;
						if (atk <= 1)
							atk += 1;
						else
							atk += atk / 2;
						pcrit = true;
					}
					else
					{
						atk = str;
					}
					c->hp = c->hp - atk;
					mp--;

					if (pcrit)
					{
						cout << name << "'s spell hit the " << c->name << " with a critical hit for " << atk << " damage." << endl;
						cout << "The " << c->name << " now has " << c->hp << "/" << c->maxhp << " hp." << endl;
						cout << name << " now has " << mp << "/" << maxmp << " mp." << endl;
					}
					else
					{
						cout << name << "'s spell hit the " << c->name << " for " << atk << " damage." << endl;
						cout << "The " << c->name << " now has " << c->hp << "/" << c->maxhp << " hp." << endl;
						cout << name << " now has " << mp << "/" << maxmp << " mp." << endl;
					}
				}
				else
				{
					mp--;
					cout << name << "'s spell failed." << endl;
					cout << name << " now has " << mp << "/" << maxmp << " mp." << endl;
				}
			}
			else
				cout << "You don't have enough mana!" << endl;
		}
		else
		{
			cout << "That wasn't an option!" << endl;
		}
	}

}

void player::move()
{
	HANDLE hIn;
	HANDLE hOut;
	COORD KeyWhere;
	COORD EndWhere;
	INPUT_RECORD InRec;
	DWORD NumRead;

	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);


	KeyWhere.X = 0;
	KeyWhere.Y = 20;
	EndWhere.X = 0;
	EndWhere.Y = 20;

	ReadConsoleInput(hIn, &InRec, 1, &NumRead);

	switch (InRec.EventType)
	{
	case KEY_EVENT:
		++KeyEvents;
		SetConsoleCursorPosition(hOut, KeyWhere);
		cout << flush;
		if (InRec.Event.KeyEvent.uChar.AsciiChar == 'w' && KeyEvents % 2 == 0)
		{
			SetConsoleCursorPosition(hOut, EndWhere);
			row--;
			plast = 'w';
		}
		else if (InRec.Event.KeyEvent.uChar.AsciiChar == 'a' && KeyEvents % 2 == 0)
		{
			SetConsoleCursorPosition(hOut, EndWhere);
			col--;
			plast = 'a';
		}
		else if (InRec.Event.KeyEvent.uChar.AsciiChar == 's' && KeyEvents % 2 == 0)
		{
			SetConsoleCursorPosition(hOut, EndWhere);
			row++;
			plast = 's';
		}
		else if (InRec.Event.KeyEvent.uChar.AsciiChar == 'd' && KeyEvents % 2 == 0)
		{
			SetConsoleCursorPosition(hOut, EndWhere);
			col++;
			plast = 'd';
		}
		else if (InRec.Event.KeyEvent.uChar.AsciiChar =='q' && KeyEvents % 2 == 0)
		{
			SetConsoleCursorPosition(hOut, EndWhere);
			quit = true;
		}
		else if (InRec.Event.KeyEvent.uChar.AsciiChar == 'i' && KeyEvents % 2 == 0)
		{
			SetConsoleCursorPosition(hOut, EndWhere);
			cout << "Name: " << name << endl;
			cout << "Strength: " << str << endl;
			cout << "Defense: " << def << endl;
			cout << "Max HP: " << maxhp << endl;
			cout << "HP: " << hp << endl;
			cout << "Max MP: " << maxmp << endl;
			cout << "MP: " << mp << endl;
			inStats = true;
		}
		break;
	}

	/*
	string dir;
	cin >> dir;
	if (!cin.fail())
	{
		if (dir == "w")
		{
			row--;
			plast = 'w';
		}
		else if (dir == "a")
		{
			col--;
			plast = 'a';
		}
		else if (dir == "s")
		{
			row++;
			plast = 's';
		}
		else if (dir == "d")
		{
			col++;
			plast = 'd';
		}
		else if (dir == "quit")
		{
			quit = true;
		}
	}
	*/

}

bool player::isAlive()
{
	if (hp > 0)
		return true;
	else
		return false;
}

char player::getLast()
{
	return plast;
}

bool player::getQuit()
{
	return quit;
}

bool player::getInStats()
{
	return inStats;
}

void player::setInStats(bool a)
{
	inStats = a;
}