#include "stdafx.h"

#include <strstream>
#include "ConchoNic.h"

using namespace std;

namespace P2_v5 {
	ConchoNic::ConchoNic(double h, double dR) :p(0, 0)
	{
		if (h <= 0) throw exception("invalid distance");
		a = h; //TODO: Is that right?
		if (dR <= 0) throw exception("invalid displacement");
		l = dR;
	}

	ConchoNic::ConchoNic(const Point &pBase, double h, double dR) : p(pBase)
	{
		if (h <= 0) throw exception("invalid distance");
		a = h;
		if (dR <= 0) throw exception("invalid displacement");
		l = dR;
	}

	ConchoNic::ConchoNic(double x0, double y0, double h, double dR) :p(x0, y0)
	{
		if (h <= 0) throw exception("invalid distance");
		a = h;
		if (dR <= 0) throw exception("invalid displacement");
		l = dR;
	}

	ConchoNic& ConchoNic::setA(double h)
	{
		if (h <= 0) throw exception("invalid distance");
		a = h;
		return *this; //TODO: WHY?!
	}

	ConchoNic& ConchoNic::setL(double dR)
	{
		if (dR <= 0) throw exception("invalid displacement");
		l = dR;
		return *this; //TODO: WHY?!
	}

	Point ConchoNic::fDec(double x) const
	{
		if (x == p.x + a) throw exception("infinity"); //TODO: Or illegal X?!
		if (abs(x - (p.x + a)) > l) throw exception("illegal argument x");
		double t = (x - p.x - a) / l;
		double dy = a*sqrt(1 - t*t) / t + l*sqrt(1 - t*t);
		return{ p.y - dy, p.y + dy };
	}

	Point ConchoNic::fPol(double angle) const
	{
		Point res;
		double ma = abs(angle);
		if (l > a) {
			double al = atan(sqrt(l*l - a*a) / a);
			if (ma <= al) {
				res.x = l + a / cos(ma);
				res.y = res.x;
			}
			else if (ma < M_PI_2) {
				res.x = l + a / cos(ma);
				res.y = l - a / cos(ma);
			}
			else if (ma <= M_PI - al) throw exception("illegal angle"); //TODO: maybe should return {0,0}?!
			else {
				res.x = l + a / cos(ma);
				res.y = res.x;
			}
		}
		if (l <= a) {
			if (ma < M_PI_2) {
				res.x = l + a / cos(ma);
				res.y = l - a / cos(ma);
			}
			else throw exception("illegal angle");
			//TODO: maybe pi/2 should return {0,0}?!
		}
		return res;
	}

	/*
	double ConchoNic::fPol(double angle) const
	{
		if (angle == M_PI_2) return 0;
		return (a / cos(angle) + l);
	}
	*/
	RArr ConchoNic::radCurv() const
	{
		RArr res = {0, 0, 0};
		res.Ra = (l + a)*(l + a) / l;
		res.Rc = (l - a)*(l - a) / l;
		if (l >= a) res.Ro = l*sqrt(l*l - a*a) / (2 * a); //TODO: or except
		return res;
	}

	double ConchoNic::area() const
	{
		if (l <= a) return 0;
		return(a*sqrt(l*l - a*a) - 2 * a*l * log((l + sqrt(l*l - a*a)) / a) + l*l*acos(a / l));
	}

	PArr ConchoNic::infPoints() const
	{
		Point f;
		PArr res;
		if (l > a) {
			res.n = 2;
			res.A[0].x = 2.35 * a + p.x;
			res.A[1].x = res.A[0].x;
			f = fDec(res.A[0].x);
			res.A[0].y = f.x;
			res.A[1].y = f.y;
		}
		if (l == a) {
			res.n = 2;
			res.A[0].x = sqrt(3) * a + p.x;
			res.A[1].x = res.A[0].x;
			f = fDec(res.A[0].x);
			res.A[0].y = f.x;
			res.A[1].y = f.y;
		}
		if (l < a) {
			res.n = 4;
			//x1
			res.A[0].x = 1.38 * a + p.x;
			res.A[1].x = res.A[0].x;
			f = fDec(res.A[0].x);
			res.A[0].y = f.x;
			res.A[1].y = f.y;
			//x2
			res.A[2].x = 0.57 * a + p.x;
			res.A[3].x = res.A[2].x;
			f = fDec(res.A[2].x);
			res.A[2].y = f.x;
			res.A[3].y = f.y;
		}
		return res;
	}
}
