// WDP_Application.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <iostream>

#include "..\DIce\Dice.h"

using namespace std;
using namespace Prog3;

void reroll(Dice &d, int np)
{
	cout << "Player #" << np << ", which ones you want to reroll? Enter numbers, 0 to end:";
	int rr[5];
	for (int rri = 0; rri < 5; rri++)
		rr[rri] = 0;
	int a;
	cin >> a;
	while (cin.good() && (a > 0) && (a < 6))
	{
		rr[a - 1] = 1;
		cin >> a;
	}
	if (!cin.good())
		return;
	Dice nd(0);
	int rrc = 0;
	for (int rri = 0; rri < 5; rri++)
		rrc += rr[rri];
	Dice rrd(rrc);
	int rrci = 0;
	for (int rri = 0; rri < 5; rri++)
	{
		if (!rr[rri]) nd += d[rri];
		else nd += rrd[rrci++];
	}
	d = nd;
}
double check(Dice &d)
{
	cout << "Your combination: ";
	int n[7];
	for (int ni = 1; ni < 7; ni++) n[ni] = 0;
	for (int i = 0; i < 5; i++) n[d[i]]++;
	//8-7
	int four = 0, tr = 0, p1 = 0, p2 = 0;
	double c = 0;
	bool f = 1;
	for (int i = 1; i <= 6; i++)
	{
		//8
		if ((n[i] != 1) && (i >= 2) && (i <= 5)) f = 0;
		if (n[i] == 5) {
			cout << "Five-of-a-Kind." << endl;
			return 400 + i; //301-306
		}
		if (n[i] == 4) four = i;
		if (n[i] == 3) tr = i;
		if ((n[i] == 2) && (p1 != 0)) p2 = i;
		if ((n[i] == 2) && (p1 == 0)) p1 = i;
		if (n[i] == 1) c += pow(0.1, i);
	}
	//7
	if (four)
	{
		cout << "Four-of-a-Kind." << endl;
		return  300 + four + c; //210... - 260...
	} 
	//6
	if (tr && p1)
	{
		cout << "Full House." << endl;
		return 200 + 10 * tr + p1 + c; //112... - 165...
	}
	//5-4
	for (int i = 5; i >= 2; i--)
	{
		if (n[i] != 1) f = 0;
	}
	if (f)
	{
		if (n[6] == 1)
		{
			cout << "Six High Straight." << endl;
			return 160;
		}
		if (n[1] == 1)
		{
			cout << "Five High Straight." << endl;
			return 150;
		}
	}
	//3
	if (tr)
	{
		cout << "Three-of-a-Kind." << endl;
		return 100 + tr + c;
	}
	//2
	if (p1 && p2)
	{
		cout << "Two Pairs." << endl;
		return p2 * 10 + p1 + c; //p2>p1, 12... - 65...
	}
	//1
	if (p1) 
	{
		cout << "Pair." << endl;
		return p1 + c; //1... - 6...
	}
	//0: <1
	cout << "Nothing." << endl;
	return c;
}

int raise()
{
	int rbet = 0;
	int drbet = 1;
	bool fl2 = true;
	while ((drbet > 0) && fl2)
	{
		if (rbet)
			cout << "Current raise: " << rbet << endl;
		cout << "Player #1, enter the amount by which you want to raise bet, enter -1 to surrender this round: ";
		cin >> drbet;
		if (!(fl2 = cin.good()))
			continue;
		if ((drbet == 0) && (rbet>0))
			continue;
		if (drbet < 0)
		{
			drbet = -1;
			continue;
		}
		rbet += drbet;
		//
		if (rbet)
			cout << "Current raise: " << rbet << endl;
		cout << "Player #2, enter the amount by which you want to raise bet, enter -1 to surrender this round: ";
		cin >> drbet;
		if (!(fl2 = cin.good()))
			continue;
		if ((drbet == 0) && (rbet>0))
			continue;
		if (drbet < 0)
		{
			drbet = -2;
			continue;
		}
		rbet += drbet;
	}
	if (!fl2) return -10;
	if (drbet < 0) return drbet;
	return rbet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Welcome to Dice Poker!" << endl;
	Dice p1(5), p2(5), np1(5), np2(5);
	int points1, points2;
	double pt1, pt2;
	int pool;
	int bet;
	int rres;
	int rn;
	bool fl1 = true;
	rn = 0;
	while ((fl1) && (rn <= 0))
	{
		cout << "Enter the number of rounds: ";
		cin >> rn;
		if ((fl1 = cin.good()) && (rn <= 0)) cout << "Wrong input!" << endl;
	}
	if (!fl1)
	{
		cout << "End of file reached. Program is stopped" << endl;
		return 1;
	}
	bet = -1;
	while ((fl1) && (bet < 0))
	{
		cout << "Enter base bet: ";
		cin >> bet;
		if ((fl1 = cin.good()) && (bet < 0)) cout << "Wrong input!" << endl;
	}
	pool = bet * 2;
	if (!fl1)
	{
		cout << "End of file reached. Program is stopped" << endl;
		return 1;
	}
	points1 = 0;
	points2 = 0;
	for (int ri = 1; ((ri <= rn) || (points1 == points2)); ri++)
	{
		cout << "________________________________________" << endl;
		if (ri <= rn)
			cout << "ROUND #" << ri << endl;
		else cout << "ADDITIONAL ROUND";
		p1.RollDice();
		cout << "Player #1:" << endl << p1;
		check(p1);
		system("pause");
		p2.RollDice();
		cout << "Player #2:" << endl << p2;
		check(p2);
		cout << "Current pool: " << pool << endl;
		rres = raise();
		if (rres == -2)
		{
			points1++;
			cout << "Player #2 surrenders the round!" << endl;
			cout << "Player #1 wins this round!" << endl;
		}
		if (rres == -1)
		{
			points2++;
			cout << "Player #1 surrenders the round!" << endl;
			cout << "Player #2 wins this round!" << endl;
		}
		if (rres >= 0)
		{
			pool += rres * 2;
			cout << "Current pool: " << pool << endl;
			reroll(p1, 1);
			if (!(fl1 = cin.good()))
				break;
			reroll(p2, 2);
			if (!(fl1 = cin.good()))
				break;
			cout << "Player #1 Results:" << endl << p1;
			pt1 = check(p1);
			cout << "Player #2 Results:" << endl << p2;
			pt2 = check(p2);
			if (pt1 > pt2)
			{
				points1++;
				cout << "Player #1 wins this round!" << endl;
			}
			if (pt2 > pt1)
			{
				points2++;
				cout << "Player #2 wins this round!" << endl;
			}
			if (pt1 == pt2)
			{
				cout << "Draw!" << endl;
			}
		}
		if (!(fl1 = cin.good()))
			break;
		cout << "Player #1: " << points1 << " points." << endl;
		cout << "Player #2: " << points2 << " points." << endl;
		cout << "Current pool: " << pool << endl;
		system("pause");
	}
	if (!fl1)
		cout << "End of file reached. Game is stopped." << endl;
	cout << "________________________________________" << endl;
	cout << "RESULTS:" << endl;
	cout << "Player #1: " << points1 << " points." << endl;
	cout << "Player #2: " << points2 << " points." << endl;
	if (points1 > points2)
		cout << "Player #1 wins!" << endl;
	if (points2 > points1)
		cout << "Player #2 wins!" << endl;
	cout << "Prize: " << pool << endl;
	return 0;
}

