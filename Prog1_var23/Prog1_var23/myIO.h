#ifndef MYIO_H
#define MYIO_H

#include <iostream>

using namespace std;

template <class Tnum>
int getNum(Tnum &a)
{
	cin >> a;
	if (!cin.good())
		return -1;
	return 0;
}

template <class Tnat>
int getNat(Tnat &a)
{
	for (;;) {
		if (getNum(a) < 0)
			return -1;
		if (a > 0)
			return 0;
		cout << "ERROR! Enter natural number:" << endl;
	}
}

#endif