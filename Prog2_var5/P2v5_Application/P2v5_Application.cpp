// P2v5_Application.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <iostream>

#include "..\P2v5_Library\ConchoNic.h"

using namespace std;
using namespace P2_v5;

#define line "________________________________________________________________________________"

int _tmain(int argc, _TCHAR* argv[])
{
	ConchoNic c;
	Point p;
	double a, l;
	bool fl1 = true;
	while (fl1)
	{
		cout << "Your conchoid:" << endl;
		p = c.getP();
		cout << "Point: (" << p.x << ", " << p.y << ")" << endl;
		cout << "Distance: " << c.getA() << endl;
		cout << "Displacement: " << c.getL() << endl;
		//radiuses
		cout << line << endl;
		RArr RA;
		RA = c.radCurv();
		cout << "Radiuses of curvature:" << endl;
		cout << "Closer: ";
		if (RA.Rc) cout << RA.Rc;
		else cout << "Don't exist";
		cout << endl << "Farther: " << RA.Ra << endl;
		cout << "Point: ";
		if (RA.Ro) cout << RA.Ro;
		else cout << "Don't exist";
		cout << endl;
		//area
		cout << line << endl;
		cout << "Area:" << c.area() << endl;
		/*
		//infl.points
		cout << line << endl;
		PArr PA = c.infPoints();
		cout << "Inflection points:";
		for (int i = 0; i < PA.n; i++)
			cout << " (" << PA.A[i].x << ", " << PA.A[i].y << ")";
		cout << endl;
		*/
		//func
		cout << line << endl;
		bool fl2 = true;
		while (fl2) {
			double x;
			Point y;
			cout << "Enter x to calculate y(x), Ctrl+Z to stop: ";
			cin >> x;
			fl2 = cin.good();
			if (!fl2) continue;
			try {
				y = c.fDec(x);
				cout << "y1 = " << y.x << "; y2 = " << y.y << endl;
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		cin.clear();
		cout << line << endl;
		fl2 = true;
		while (fl2) {
			double angle;
			Point r;
			cout << "Enter angle to calculate r(angle), Ctrl+Z to stop: ";
			cin >> angle;
			fl2 = cin.good();
			if (!fl2) continue;
			try {
				r = c.fPol(angle);
				if (r.x == r.y) cout << "r = " << r.x << endl;
				else cout << "r1 = " << r.x << "; r2 = " << r.y << endl;
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
			/*
			double angle;
			cout << "Enter angle to calculate r(angle), Ctrl+Z to stop: ";
			cin >> angle;
			fl2 = cin.good();
			if (!fl2) continue;
			cout << "r = " << c.fPol(angle) << endl;
			*/
		}
		cin.clear();
		cout << line << endl;
		cout << line << endl;
		cout << "Enter new x, y, a, l to continue; Ctrl+Z to stop: ";
		cin >> p.x >> p.y >> a >> l;
		if (cin.good()) {
			c.setP(p);
			try {
				c.setA(a);
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
			try {
				c.setL(l);
			}
			catch (exception &ex)
			{
				cout << ex.what() << endl;
			}
		}
		else fl1 = 0;
	}
	//system("pause");
	return 0;
}

