// P3_1_Dialog.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "..\P3_1_Library\Hex.h"

#define HC 0x5F
#define LINE "________________________________________________________________________________"

using namespace std;
using namespace P3_1;

int _tmain(int argc, _TCHAR* argv[])
{
	Hex a(HC);
	Hex b, c;
	bool fl1 = true;
	while (fl1)
	{
		cout << "Your number: ";
		a.hexout(cout);
		cout << endl;
		cout << "Parity: ";
		if (a.isEven())
			cout << "yes" << endl;
		else
			cout << "no" << endl;
		bool fl2 = true;
		while (fl2)
		{
			cout << "Enter another number to add, Ctrl+Z to stop: ";
			b.hexin(cin);
			fl2 = !cin.eof();
			if (!fl2)
				continue;
			if (cin.fail())
			{
				cout << "Wrong input!" << endl;
				cin.clear();
				continue;
			}
			try
			{
				c = a.sum(b);
				cout << "Result: ";
				c.hexout(cout);
				cout << endl;
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		cin.clear();
		cout << LINE << endl;
		fl2 = true;
		while (fl2)
		{
			cout << "Enter offset to move your number (left), Ctrl+Z to stop: ";
			int d;
			cin >> d;
			fl2 = cin.good();
			if (!fl2)
				continue;
			try
			{
				a.moveL(d);
				cout << "Result: ";
				a.hexout(cout);
				cout << endl;
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		cin.clear();
		cout << LINE << endl;
		fl2 = true;
		while (fl2)
		{
			cout << "Enter offset to move your number (right), Ctrl+Z to stop: ";
			int d;
			cin >> d;
			fl2 = cin.good();
			if (!fl2)
				continue;
			try
			{
				a.moveR(d);
				cout << "Result: ";
				a.hexout(cout);
				cout << endl;
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		cin.clear();
		cout << LINE << endl;
		fl2 = true;
		while (fl2)
		{
			cout << "Enter another number to compare, Ctrl+Z to stop: ";
			b.hexin(cin);
			fl2 = !cin.eof();
			if (!fl2)
				continue;
			if (cin.fail())
			{
				cout << "Wrong input!" << endl;
				cin.clear();
				continue;
			}
			cout << "Your number is ";
			if (a.isBigger(b))
				cout << "bigger." << endl;
			if (a.isEqual(b))
				cout << "equal." << endl;
			if (a.isLesser(b))
				cout << "lesser." << endl;
		}
		cin.clear();
		cout << LINE << endl;
		cout << LINE << endl;
		cout << "Enter new number to continue, Ctrl+Z to stop: ";
		a.hexin(cin);
		fl1 = cin.good();
	}
	return 0;
}

