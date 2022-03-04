// P4v25_ConsoleGame.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#define USE_TEMPLATES 0

#if (USE_TEMPLATES)
#include "../P4v25_Library_Templates/Level.h"
#else
#include "../P4v25_Library/Level.h"
#endif

using namespace std;

#if (USE_TEMPLATES)
using namespace P4v25_T;
#else
using namespace P4v25;
#endif

//И В ЧЁМ БЫЛА ПРОБЛЕМА С stdafx?!

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Welcome!" << endl;
	char buf;
	bool flagBreak;
	try
	{
		//LOADING
		//DEBUG: Map and Config files are loaded as defaults.
		string mapFname = string(MAP_FOLDER) + string("/") + string(DEFAULT_MAP_FILE);
		string configFname = string(CONFIG_FOLDER) + string("/") + string(DEFAULT_CONFIG_FILE);
		string saveFname;
		cout << "Enter L to load from savefile or N to begin a new game: ";
		cin.ignore(cin.rdbuf()->in_avail());
		flagBreak = 0;
		while (!flagBreak)
		{
			buf = cin.get();
			if (!cin.good())
				throw exception("input_error");
			switch (buf)
			{
			case 'L':
				cout << "Enter save file name: ";
				cin >> saveFname;
				if (!cin.good())
					throw exception("input_error");
				saveFname = string(SAVE_FOLDER) + string("/") + saveFname;
				flagBreak = 1;
				break;
			case 'N':
				saveFname = string(SAVE_FOLDER) + string("/") + string(DEFAULT_SAVE_FILE);
				flagBreak = 1;
				break;
			}
		}
		Level game (mapFname, configFname, saveFname);
		//GAME
		char buf1;
		bool flagBreak1;
		int x, y, t, rc;
		string bname;
		string saveFnameNew;
		int trc;
		double healing;
		while (1)
		{
			try
			{
				cout << game;
				cout << "T - turn, B - build, U - upgrade, R - repair (wall), S - save and exit, E - exit" << endl;
				cin.ignore(cin.rdbuf()->in_avail());
				buf = '0';
				flagBreak = 0;
				while (!flagBreak)
				{
					buf = cin.get();
					if (!cin.good())
						throw exception("input_error");
					switch (buf)
					{
					case 'T':
						trc = game.turn();
						switch (trc)
						{
						case 0:
							cout << "Time: " << game.getTimeG() << endl;
							break;
						case -1:
							cout << "YOU LOSE" << endl;
							system("pause");
							return -1;
							break;
						case 1:
							cout << "YOU WIN" << endl;
							system("pause");
							return 1;
							break;
						}
						flagBreak = 1;
						break;
					case 'B':
						cout << "Choose, what to build: T - tower, W - wall, Q - quit (from build menu): ";
						cin.ignore(cin.rdbuf()->in_avail());
						buf1 = '0';
						flagBreak1 = 0;
						while (!flagBreak1)
						{
							buf1 = cin.get();
							if (!cin.good())
								throw exception("input_error");
							switch (buf1)
							{
							case 'W':
								t = 11;
								flagBreak1 = 1;
								break;
							case 'T':
								t = 12;
								flagBreak1 = 1;
								break;
							case 'Q':
								flagBreak1 = 1;
								flagBreak = 1;
								break;
							}
						}
						if (flagBreak) //used for Quit
							continue;
						cout << "Enter position (x y). Enter negative to quit: ";
						cin >> x >> y;
						if (!cin.good())
							throw exception("input_error");
						if ((x < 0) || (y < 0))
						{
							flagBreak = 1;
							break;
						}
						cout << "Enter building name: ";
						cin >> bname;
						if (!cin.good())
							throw exception("input_error");
						rc = game.build(Pos(x, y), t, bname);
						switch (rc)
						{
						case 0:
							cout << "Not existing position!" << endl;
							break;
						case -1:
							cout << "Wrong place (bad terrain/another building)!" << endl;
							break;
						case -2:
							cout << "Not enough gold!" << endl;
							break;
						case -3:
							cout << "Blocking path for light infantry!" << endl;
							break;
						case -4:
							cout << "Enemies at position!" << endl;
							break;
						case 1:
							cout << "Done!" << endl;
							break;
						default:
							throw exception("unexpected_error");
							break;
						}
						flagBreak = 1;
						break;
					case 'U':
						cout << "Enter position (x y). Enter negative to quit: ";
						int x, y, rc;
						cin >> x >> y;
						if (!cin.good())
							throw exception("input_error");
						if ((x < 0) || (y < 0))
						{
							flagBreak = 1;
							break;
						}
						rc = game.upgrade(Pos(x, y));
						switch (rc)
						{
						case 0:
							cout << "Not existing position!" << endl;
							break;
						case -1:
							cout << "Wrong place (empty/enemy building) or not upgradable building (wall)!" << endl;
							break;
						case -2:
							cout << "Not enough gold!" << endl;
							break;
						case -3:
							cout << "Maximum level is already reached!" << endl;
							break;
						case 1:
							cout << "Done!" << endl;
							break;
						default:
							throw exception("unexpected_error");
							break;
						}
						flagBreak = 1;
						break;
					case 'R':
						cout << "Enter position (x y). Enter negative to quit: ";
						cin >> x >> y;
						if (!cin.good())
							throw exception("input_error");
						if ((x < 0) || (y < 0))
							continue;
						cout << "Enter healing value: ";
						cin >> healing;
						if (!cin.good())
							throw exception("input_error");
						rc = game.repair(Pos(x, y), healing);
						switch (rc)
						{
						case 0:
							cout << "Not existing position!" << endl;
							break;
						case -1:
							cout << "There is no any walls!" << endl;
							break;
						case -2:
							cout << "Not enough gold!" << endl;
							break;
						case -3:
							cout << "Negative healing value!" << endl;
							break;
						case 1:
							cout << "Done!" << endl;
							break;
						default:
							throw exception("unexpected_error");
							break;
						}
						flagBreak = 1;
						break;
					case 'S':
						cout << "Enter save file name: ";
						cin >> saveFnameNew;
						if (!cin.good())
							throw exception("input_error");
						game.save(string(SAVE_FOLDER) + string("/") + saveFnameNew);
						cout << "Game was stopped by player." << endl;
						system("pause");
						return 0;
					case 'E':
						cout << "Game was stopped by player." << endl;
						system("pause");
						return 0;
					}
				}
			}
			catch (exception &ex)
			{
				game.save(string(SAVE_FOLDER) + string("/") + string(CRASH_SAVE_FILE));
				cout << "Game was stopped. ERROR: " << ex.what() << endl;
				cout << "Current game state was saved to this file: " << CRASH_SAVE_FILE << endl;
				system("pause");
				return 0;
			}
			
		}
	}
	catch (exception &ex)
	{
		cout << "Game wasn't loaded. ERROR: " << ex.what() << endl;
		system("pause");
		return 0;
	}
}