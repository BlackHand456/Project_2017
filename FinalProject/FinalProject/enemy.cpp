#include "enemy.h"

char elast = 'a';
bool ecrit = false;

void enemy::attack(character *c)
{
	ecrit = false;
	random_device rd;
	uniform_int_distribution<int> d2(1, 2);
	//uniform_int_distribution<int> d3(1, 3);
	uniform_int_distribution<int> d10(1, 10);

			if (d2(rd) == 1)
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
					ecrit = true;
				}
				if (atk <= 0)
					atk = 1;
				c->hp = c->hp - atk;
				if (ecrit)
				{
					cout << "The " << name << " hit " << c->name << " with a critical hit for " << atk << " damage." << endl;
					cout << c->name << " now has " << c->hp << "/" << c->maxhp << " hp." << endl;
				}
				else
				{
					cout << "The " << name << " hit " << c->name << " for " << atk << " damage." << endl;
					cout << c->name << " now has " << c->hp << "/" << c->maxhp << " hp." << endl;
				}
			}
			else
				cout << "The " << name << "'s attack missed." << endl;
}

void enemy::move()
{
	random_device rd;
	uniform_int_distribution<int> d4(1, 4);

	int dir = d4(rd);
		if (dir == 1)
		{
			row--;
			elast = 'w';
		}
		else if (dir == 2)
		{
			col--;
			elast = 'a';
		}
		else if (dir == 3)
		{
			row++;
			elast = 's';
		}
		else if (dir == 4)
		{
			col++;
			elast = 'd';
		}
}

bool enemy::isAlive()
{
	if (hp > 0)
		return true;
	else
		return false;
}

char enemy::getLast()
{
	return elast;
}