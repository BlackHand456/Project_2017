#include "main.h"

player *p1;

enemy *inBattle;
int inBattleIndex;

enemy *boss = new enemy("Troll", 8, 8, 15, 15, 0, 0, 0, 0, false);

string currentMapName = "map1";

char currentMap[20][40];
vector<enemy*> enemyList;
char battleMap[20][80];

vector<chest*> chestList;

int KeyEvents2 = 0;

int enemyNum = 10;

bool atExit = false;
bool bossDead = false;
bool atBoss = false;

int startScreen()
{
	bool choosing = true;
	while (choosing)
	{
		int choice;
		cout << "<1> New Game\n"
			"<2> Load\n"
			"<3> Quit\n" << endl;
		cin >> choice;
		if (!cin.fail())
		{
			if (choice == 1)
			{
				cout << "Starting new game..." << endl;
				choosing = false;
				return 1;
			}
			else if (choice == 2)
			{
				cout << "Loading saved data..." << endl;
				choosing = false;
				return 2;
			}
			else if (choice == 3)
			{
				cout << "Goodbye!" << endl;
				choosing = false;
				return 3;
			}
			else
			{
				cout << "That wasn't an option!" << endl;
			}
		}
		else
		{
			cout << "Only use valid inputs!" << endl;
			choosing = false;
		}
	}
	return 0;
}

void saveChar()
{
	fstream f;

	f.open(p1->name + ".txt", ios::out);

	if (f.is_open())
	{
		f << p1->name << endl;
		f << p1->str << endl;
		f << p1->def << endl;
		f << p1->maxhp << endl;
		f << p1->hp << endl;
		f << p1->maxmp << endl;
		f << p1->mp << endl;
		f << p1->row << endl;
		f << p1->col << endl;
	}

	f.close();
}

void createChar()
{
	string name;
	cout << "Please enter a character name." << endl;
	cin >> name;
	p1 = new player(name, 5, 5, 10, 10, 5, 5, 17, 1, false);
	saveChar();
}

void loadChar(string name)
{
	fstream f;
	string name2;
	int str;
	int def;
	int maxhp;
	int hp;
	int maxmp;
	int mp; 
	int row;
	int col;

	f.open(name + ".txt", ios::in);

	if (f.is_open())
	{
		f >> name2;
		f >> str;
		f >> def;
		f >> maxhp;
		f >> hp;
		f >> maxmp;
		f >> mp;
		f >> row;
		f >> col;
	}
	else
	{
		cout << "That character doesn't exist. You have to create one." << endl;
		createChar();
		return;
	}

	f.close();
	p1 = new player(name2, str, def, maxhp, hp, maxmp, mp, row, col, false);
}

int menu()
{
	int choice = startScreen();
	if (choice == 1)
	{
		createChar();
		return 1;
	}
	else if (choice == 2)
	{
		string name;
		cout << "Enter the name of the character's data you want to load." << endl;
		cin >> name;
		loadChar(name);
		return 1;
	}
	else
	{
		return 0;
	}
}

void intro()
{
	cout << "Welcome..." << endl;
	cout << "You are trapped in the dungeon and must fight your way out and survive.\n"
		"Go to the 'E' on the map to move to the next level." << endl;
	cout << "Key: \n"
		"<*> - Player\n"
		"<@> - Enemy\n"
		"<#> - Chest\n"
		"<%> - Key\n"
		"<(> - Locked Door\n"
		"<$> - Boss\n"
		"<S> - Start\n"
		"<E> - Exit" << endl;
	cout << "Controls: \n"
		"<w,a,s,d> - Move\n"
		"<q> - Quit\n"
		"<i> - Toggle Stats" << endl;
}

void openedChest(chest *c)
{
	if (c->stat == "str")
	{
		if (c->val > p1->str)
		{
			p1->str = c->val;
			cout << p1->name << " found a " << c->itemName << "!" << endl;
			cout << p1->name << " equipped the " << c->itemName << "!" << endl;
		}
		else
		{
			cout << p1->name << " found a " << c->itemName << "!" << endl;
			cout << p1->name << "'s current weapon is better, so you leave the item where it is." << endl;
		}
	}
	else if (c->stat == "def")
	{
		if (c->val > p1->def)
		{
			p1->def = c->val;
			cout << p1->name << " found a " << c->itemName << "!" << endl;
			cout << p1->name << " equipped the " << c->itemName << "!" << endl;
		}
		else
		{
			cout << p1->name << " found a " << c->itemName << "!" << endl;
			cout << p1->name << "'s current shield is better, so you leave the item where it is." << endl;
		}
	}
	else if (c->stat == "hp")
	{
		p1->hp += c->val;
		cout << p1->name << " found a " << c->itemName << "!" << endl;
		cout << p1->name << " drank it and restored their health!" << endl;
		if (p1->hp > p1->maxhp)
			p1->hp = p1->maxhp;
	}
	else
	{
		p1->mp += c->val;
		cout << p1->name << " found a " << c->itemName << "!" << endl;
		cout << p1->name << " drank it and restored their mana!" << endl;
		if (p1->mp > p1->maxmp)
			p1->mp = p1->maxmp;
	}
	this_thread::sleep_for(chrono::milliseconds(1000));
}

void makeMap()
{
	fstream f;

	f.open(currentMapName + ".txt", ios::in);

	while(f.is_open())
	{
		for (int i = 0; i < 20; i++)
		{
			string temp;
			getline(f, temp);
			if (temp == "")
				break;
			for (int x = 0; x < 40; x++)
			{
				if (i == p1->row && x == p1->col)
				{
					if (temp[x] == '$' && !bossDead)
					{
						cout << "Oh no! You encountered a troll guarding the exit!" << endl;
						this_thread::sleep_for(chrono::milliseconds(800));
						atBoss = true;
					}
					if (temp[x] == '%')
					{
						cout << "You recieved a key!" << endl;
						this_thread::sleep_for(chrono::milliseconds(500));
						p1->key = true;
					}
					if (temp[x] == 'E')
						atExit = true;
					if (temp[x] == '#')
					{
						chest *ctemp;
						for (int y = 0; y < chestList.size(); y++)
						{
							if (i == chestList.at(y)->row && x == chestList.at(y)->col)
							{
								ctemp = chestList.at(y);
								break;
							}
						}
						if (ctemp->opened == false)
						{
							openedChest(ctemp);
							ctemp->opened = true;
						}
						else
						{
							cout << "You already opened this chest!" << endl;
							this_thread::sleep_for(chrono::milliseconds(500));
						}
					}
					if (temp[x] == '|' || temp[x] == '-' || (temp[x] == '(' && p1->key == false) || (temp[x] == '~' && bossDead == false))
					{
						if (p1->getLast() == 'w')
							p1->row++;
						else if(p1->getLast() == 'a')
							p1->col++;
						else if(p1->getLast() == 's')
							p1->row--;
						else
							p1->col--;
						makeMap();
					}
					else
					{
						currentMap[i][x] = '*';
					}
				}
				else
				{
					bool enemyLocation = false;
					enemy *etemp;
					for (int y = 0; y < enemyList.size(); y++)
					{
						if (i == enemyList.at(y)->row && x == enemyList.at(y)->col)
						{
							enemyLocation = true;
							etemp = enemyList.at(y);
							break;
						}
					}
					if (enemyLocation == true)
					{
						if (temp[x] == '|' || temp[x] == '-')
						{
							if (etemp->getLast() == 'w')
								etemp->row++;
							else if (etemp->getLast() == 'a')
								etemp->col++;
							else if (etemp->getLast() == 's')
								etemp->row--;
							else
								etemp->col--;
							//makeMap();
						}
						else
						{
							currentMap[i][x] = '@';
						}
					}
					else
					{
						currentMap[i][x] = temp[x];
					}
				}
			}
		}
		f.close();
	}
	if (f.is_open())
		f.close();
}
//fix enemy movement

void drawMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int x = 0; x < 40; x++)
		{
			cout << currentMap[i][x];
		}
		cout << endl;
	}
}

enemy* makeEnemy()
{
	random_device rd;
	uniform_int_distribution<int> dr(0, 19);
	uniform_int_distribution<int> dc(0, 39);
	uniform_int_distribution<int> ds(1, 6);
	uniform_int_distribution<int> dn(1, 3);

	string name;

	int nameNum = dn(rd);
	if (nameNum == 1)
		name = "Goblin";
	else if (nameNum == 2)
		name = "Vampire Bat";
	else
		name = "Skeleton";
	
	int str = ds(rd);
	int def = ds(rd);
	int maxhp = 5;
	int hp = 5;
	int maxmp = 5;
	int mp = 5;
	int row = dr(rd);
	int col = dc(rd);
	enemy *e = new enemy(name, str, def, maxhp, hp, maxmp, mp, row, col, false);
	return e;
}

chest* makeChest(int row, int col)
{
	random_device rd;
	uniform_int_distribution<int> dr(0, 19);
	uniform_int_distribution<int> dc(0, 39);
	uniform_int_distribution<int> ds(1, 4);
	uniform_int_distribution<int> dv(1, 390);

	string stat;
	int statNum = ds(rd);
	if (statNum == 1)
		stat = "str";
	else if (statNum == 2)
		stat = "def";
	else if (statNum == 3)
		stat = "hp";
	else
		stat = "mp";

	int val;
	int valNum = dv(rd);
	if (stat == "str" || stat == "def")
	{
		if (valNum < 200)
			val = 6;
		else if (valNum >= 200 && valNum < 300)
			val = 7;
		else if (valNum >= 300 && valNum < 350)
			val = 8;
		else if (valNum >= 350 && valNum < 375)
			val = 9;
		else
			val = 10;
	}
	else
	{
		if (valNum < 200)
			val = 1;
		else if (valNum >= 200 && valNum < 300)
			val = 2;
		else if (valNum >= 300 && valNum < 350)
			val = 3;
		else if (valNum >= 350 && valNum < 375)
			val = 4;
		else
			val = 5;
	}

	string name;
	if (stat == "str")
	{
		name = "Sword";
		if (val == 6)
			name = "Wooden " + name;
		else if (val == 7)
			name = "Steel Short" + name;
		else if (val == 8)
			name = "Steel Broad" + name;
		else if (val == 9)
			name = "Dragon " + name;
		else
			name = "Cursed Dragon " + name;
	}
	else if (stat == "def")
	{
		name = "Shield";
		if (val == 6)
			name = "Wooden " + name;
		else if (val == 7)
			name = "Steel Round" + name;
		else if (val == 8)
			name = "Steel Tower" + name;
		else if (val == 9)
			name = "Dragon " + name;
		else
			name = "Cursed Dragon " + name;
	}
	else if (stat == "hp")
	{
		name = "Health Potion";
		if (val == 1)
			name = "Small " + name;
		else if (val == 2)
			name = name;
		else if (val == 3)
			name = "Greater " + name;
		else if (val == 4)
			name = "Grand " + name;
		else
			name = "Super " + name;
	}
	else
	{
		name = "Mana Potion";
		if (val == 1)
			name = "Small " + name;
		else if (val == 2)
			name = name;
		else if (val == 3)
			name = "Greater " + name;
		else if (val == 4)
			name = "Grand " + name;
		else
			name = "Super " + name;
	}
	
	chest *c = new chest(name, stat, val, row, col, false);
	return c;
}

void createEnemies()
{
	for (int i = 0; i < enemyNum; i++)
	{
	enemy *e = makeEnemy();
	enemyList.push_back(e);
	}
}

void createChests()
{
		fstream f;

		f.open(currentMapName + ".txt", ios::in);

		while (f.is_open())
		{
			for (int i = 0; i < 20; i++)
			{
				string temp;
				getline(f, temp);
				if (temp == "")
					break;
				for (int x = 0; x < 40; x++)
				{
					if (temp[x] == '#')
					{
						chest *c = makeChest(i, x);
						chestList.push_back(c);
					}
				}
			}
			f.close();
		}
		if (f.is_open())
			f.close();
}

void enemiesMove()
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		enemy *temp = enemyList.at(i);
		temp->move();
	}
}
//Enemies are moving twice for each time I move.

bool checkBattle()
{
	int row = p1->row;
	int col = p1->col;
	for (int i = 0; i < enemyList.size(); i++)
	{
		enemy *temp = enemyList.at(i);
		if (row == temp->row && col == temp->col)
		{
			inBattle = temp;
			inBattleIndex = i;
			return true;
		}
	}
	return false;
}

void makeBattleMap()
{
	fstream f2;
	f2.open(inBattle->name + "Battle.txt", ios::in);

	while (f2.is_open())
	{
		for (int i = 0; i < 20; i++)
		{
			string temp;
			getline(f2, temp);
			if (temp == "")
				break;
			for (int x = 0; x < 80; x++)
			{
				battleMap[i][x] = temp[x];
			}
		}
		f2.close();
	}
	if (f2.is_open())
		f2.close();
}

void drawBattleMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int x = 0; x < 80; x++)
		{
			cout << battleMap[i][x];
		}
		cout << endl;
	}
}

void fight()
{
	makeBattleMap();
	drawBattleMap();

	string eName = inBattle->name;

	while (p1->isAlive() && inBattle->isAlive())
	{
		p1->attack(inBattle);

		if (!inBattle->isAlive())
		{
			if(eName != "Troll")
			enemyList.erase(enemyList.begin()+inBattleIndex);
			else
			break;
		}
		else

		inBattle->attack(p1);
	}
	if (p1->isAlive() && eName == "Troll")
	{
		cout << "You managed to defeat the powerful " << eName << "!!!" << endl;
		bossDead = true;
	}
	else if (p1->isAlive())
	{
		cout << "You defeated the " << eName << "." << endl;
	}
	else
	{
		cout << "The " << eName << " defeated you. Better luck next time!" << endl;
	}
	this_thread::sleep_for(chrono::milliseconds(800));
}

void clearConsole()
{
	system("cls");
}

void statScreen()
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
		++KeyEvents2;
		SetConsoleCursorPosition(hOut, KeyWhere);
		cout << flush;
		if (InRec.Event.KeyEvent.uChar.AsciiChar == 'i' && KeyEvents2 % 2 == 0)
		{
			p1->setInStats(false);
		}
		break;
	}
}

int main()
{
	if (menu() < 1)
	{
		return 0;
	}

	intro();

	this_thread::sleep_for(chrono::milliseconds(5000));

	createEnemies();
	createChests();

	
	while (p1->isAlive())
	{
		this_thread::sleep_for(chrono::milliseconds(100));

		makeMap();

		clearConsole();

		drawMap();

		p1->move();

		while (p1->getInStats())
		{
			statScreen();
		}

		if (p1->getQuit())
		{
			saveChar();
			break;
		}

		if (atExit)
		{
			if (currentMapName == "map1")
			{
				currentMapName = "map2";
				enemyNum = 20;
			}
			else if (currentMapName == "map2")
				currentMapName = "map3";
			else
				break;

			p1->key = false;
			atExit = false;
			createEnemies();
			createChests();
		}

		if (checkBattle())
		{
			fight();
		}

		if (atBoss)
		{
			inBattle = boss;
			fight();
		}

		if (!p1->isAlive())
			break;

		enemiesMove();

		if (checkBattle())
		{
			fight();
		}
	}
	if (!p1->isAlive())
	{
		cout << "GAME OVER!" << endl;
		cout << "Sorry, you died." << endl;
	}
	else
	{
		if (p1->getQuit())
		{
			cout << "Saving data.." << endl;
			cout << "Thanks for playing!" << endl;
			cout << "Quitting..." << endl;
		}
		else
		{
			cout << "Congratulations! You made it out of the dungeon alive!" << endl;
			cout << "You Win!" << endl;
		}
	}
}