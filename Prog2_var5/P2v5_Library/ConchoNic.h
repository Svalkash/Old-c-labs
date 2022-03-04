#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace P2_v5 {
	struct Point {
		double x, y;
		Point(double x0 = 0, double y0 = 0) :x(x0), y(y0){}
	};

	struct PArr {
		int n;
		Point A[4];
	};

	struct RArr {
		double Ra, Rc, Ro; //Ro = -1 =>no point
	};

	class ConchoNic
	{
	private:
		Point p; //center
		double a; //distance, >0
		double l; //displacement, >0
	public:
		ConchoNic(double h = 1, double dR = 2); //point 0,0
		ConchoNic(const Point &pBase, double h = 1, double dR = 2); //withpoint
		ConchoNic(double x0, double y0, double h, double dR); //fullydefined
		//set
		ConchoNic& setP(const Point &p0){ p = p0; return *this; } //WHY,BLEAT?!
		ConchoNic& setA(double h);
		ConchoNic& setL(double dR);
		//get
		Point getP() const{ return p; }
		double getA() const{ return a; }
		double getL() const{ return l; }
		//others
		Point fDec(double x) const; //y(x), y*2
		Point fPol(double angle) const; //r(angle)
		//double fPol(double angle) const;
		RArr radCurv() const;
		double area() const;
		//TODO: return(a*sqrt(l*l - a*a) - 2 * a*l * log((l + sqrt(l*l - a*a)) / a) + l*l*acos(a / l));
		PArr infPoints() const; //TODO: is it right?
	};
}
