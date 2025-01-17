// P3_2_Dialog.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include "..\P3_2_Library\Hex.h"

#define HC 0x12F
#define LINE "________________________________________________________________________________"

using namespace std;
using namespace P3_2;

int _tmain(int argc, _TCHAR* argv[])
{
	Hex a(HC);
	Hex b, c, d;
	bool fl1 = true;
	while (fl1)
	{
		cout << "Your number: " << a << endl;
		cout << "Parity: ";
		if (a.isEven())
			cout << "yes" << endl;
		else
			cout << "no" << endl;
		bool fl2 = true;
		while (fl2)
		{
			cout << "Enter another number to add, Ctrl+Z to stop: ";
			cin >> b;
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
				c = a + b;
				cout << "Result: " << c << endl;
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
				a <<= d;
				cout << "Result: " << a << endl;
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
				a >>= d;
				cout << "Result: " << a << endl;
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
			cin >> b;
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
			if (a > b)
				cout << "bigger." << endl;
			if (a == b)
				cout << "equal." << endl;
			if (a < b)
				cout << "lesser." << endl;
		}
		cin.clear();
		cout << LINE << endl;
		cout << LINE << endl;
		cout << "Enter new number to continue, Ctrl+Z to stop: ";
		cin >> a;
		fl1 = cin.good();
	}
	return 0;
}

